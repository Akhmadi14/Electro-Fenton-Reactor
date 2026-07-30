// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"
#include "SpektroShared.h"
#include <string.h>

static bool queueDegradaMeasurement(int reaksiId, int tMenit) {
    SpektroCmd cmd = { SPEKTRO_CMD_DEGRADASI, reaksiId, tMenit };

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        degradaSync.busy     = 1;
        degradaSync.produced = 0;
        degradaSync.sent     = 0;
        degradaSync.reaksiId = reaksiId;
        degradaSync.t_menit  = tMenit;
        degradaSync.sentAtMs = 0;
        xSemaphoreGive(mutexSystemState);
    }

    if (xQueueSend(queueSpektroCmd, &cmd, 0) == pdPASS) {
        return true;
    }

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        degradaSync.busy = 0;
        xSemaphoreGive(mutexSystemState);
    }
    return false;
}

static bool queueSpektrumScan(int reaksiId) {
    SpektroCmd cmd = { SPEKTRO_CMD_SPEKTRUM, reaksiId, 0 };

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        spektrumSync.busy     = 1;
        spektrumSync.produced = 0;
        spektrumSync.sent     = 0;
        spektrumSync.reaksiId = reaksiId;
        spektrumSync.sentAtMs = 0;
        xSemaphoreGive(mutexSystemState);
    }

    if (xQueueSend(queueSpektroCmd, &cmd, 0) == pdPASS) {
        return true;
    }

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        spektrumSync.busy = 0;
        xSemaphoreGive(mutexSystemState);
    }
    return false;
}

static bool wasDegradaSent(int reaksiId, int tMenit, unsigned long nowMs) {
    bool ready = false;
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        ready = (degradaSync.sent &&
                 degradaSync.reaksiId == reaksiId &&
                 degradaSync.t_menit == tMenit &&
                 (nowMs - degradaSync.sentAtMs) >= SPEKTRO_SERIAL_GUARD_MS);
        xSemaphoreGive(mutexSystemState);
    }
    return ready;
}

static bool wasSpektrumSent(int reaksiId, unsigned long nowMs) {
    bool ready = false;
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        ready = (spektrumSync.sent &&
                 spektrumSync.reaksiId == reaksiId &&
                 (nowMs - spektrumSync.sentAtMs) >= SPEKTRUM_SERIAL_GUARD_MS);
        xSemaphoreGive(mutexSystemState);
    }
    return ready;
}

static void activateStep(const ScheduleArray &schedules,
                         int stepIdx,
                         unsigned long &scheduleStartTime,
                         int &nextDegradaMin,
                         int &lastQueuedDegradaMin,
                         bool &spektroTriggered,
                         bool &waitingFinalDegrada,
                         int &finalDegradaMin) {
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        currentActiveParam   = schedules.schedules[stepIdx];
        currentRemainingTime = currentActiveParam.durasi;
        xSemaphoreGive(mutexSystemState);
    }

    scheduleStartTime     = millis();
    nextDegradaMin        = 0;
    lastQueuedDegradaMin  = -1;
    spektroTriggered      = false;
    waitingFinalDegrada   = false;
    finalDegradaMin       = -1;
}

static void finishExecution(void) {
    Serial.println("EXECUTE_DONE");
    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        currentSystemState = STATE_IDLE;
        currentRemainingTime = 0;
        memset(&currentActiveParam, 0, sizeof(ScheduleParam));
        xSemaphoreGive(mutexSystemState);
    }
}

void taskTimer(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Task Logic & Timer Siap!");

    // Status awal: inisialisasi input (reaktor belum diisi)
    currentSystemState = STATE_INISIALISASI_INPUT;

    ScheduleArray currentSchedules = {0};
    int currentStep = 0;

    int forceStopCmd = 0;
    unsigned long scheduleStartTime = 0;

    /* Tracking pengukuran degradasi per step */
    int  nextDegradaMin       = 0;
    int  lastQueuedDegradaMin = -1;
    bool spektroTriggered     = false;
    bool waitingFinalDegrada  = false;
    int  finalDegradaMin      = -1;

    // ==== LOOPING TASK ====
    for(;;) {
        // Force stop hanya berlaku saat STATE_EKSEKUSI
        if (currentSystemState == STATE_EKSEKUSI) {
            if(xQueueReceive(queueForceStop, &forceStopCmd, 0) == pdTRUE) {
                if (forceStopCmd == 1) {
                    finishExecution();
                }
            }
        }

        /* State machine jadwal — hanya aktif pada IDLE dan EKSEKUSI.
         * State lain (INISIALISASI_INPUT, INISIALISASI_EKSEKUSI, FLUSHING)
         * dikelola oleh taskManajemenPompa via taskKomunikasi.            */
        if (currentSystemState == STATE_IDLE) {
            if(xQueueReceive(queueScheduleParam, &currentSchedules, 0) == pdTRUE) {
                if (currentSchedules.count > 0) {
                    currentStep = 0;

                    if(xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                        currentSystemState = STATE_EKSEKUSI;
                        xSemaphoreGive(mutexSystemState);
                    }
                    activateStep(currentSchedules,
                                 currentStep,
                                 scheduleStartTime,
                                 nextDegradaMin,
                                 lastQueuedDegradaMin,
                                 spektroTriggered,
                                 waitingFinalDegrada,
                                 finalDegradaMin);
                }
            }
        }
        else if (currentSystemState == STATE_EKSEKUSI) {
            unsigned long currentTime = millis();
            unsigned long elapsedSec  = (currentTime - scheduleStartTime) / 1000;
            ScheduleParam *aktif = &currentSchedules.schedules[currentStep];
            int durasiAktif = aktif->durasi;

            /* --- Step spektro: tunggu sampai payload spektrum benar-benar terkirim --- */
            if (strcmp(aktif->reaksi, "spektro") == 0) {
                if (!spektroTriggered) {
                    if (queueSpektrumScan(aktif->id)) {
                        spektroTriggered = true;
                    }
                }

                if (wasSpektrumSent(aktif->id, currentTime)) {
                    currentStep++;
                    if (currentStep < currentSchedules.count) {
                        activateStep(currentSchedules,
                                     currentStep,
                                     scheduleStartTime,
                                     nextDegradaMin,
                                     lastQueuedDegradaMin,
                                     spektroTriggered,
                                     waitingFinalDegrada,
                                     finalDegradaMin);
                    } else {
                        finishExecution();
                    }
                } else {
                    if(xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                        currentRemainingTime = 0;
                        xSemaphoreGive(mutexSystemState);
                    }
                }
            } else {
                /* --- Trigger pengukuran degradasi tiap 5 menit --- */
                if (aktif->ukur_degradasi && !waitingFinalDegrada) {
                    int elapsedMin = (int)(elapsedSec / 60);
                    if (elapsedMin >= nextDegradaMin) {
                        if (queueDegradaMeasurement(aktif->id, nextDegradaMin)) {
                            lastQueuedDegradaMin = nextDegradaMin;
                            nextDegradaMin += 5;
                        }
                    }
                }

                /* --- Cek apakah step selesai --- */
                if (elapsedSec >= (unsigned long)durasiAktif) {
                    if (aktif->ukur_degradasi) {
                        /* Selalu pastikan titik akhir (mis. 35, 53) ikut masuk. */
                        if (!waitingFinalDegrada) {
                            finalDegradaMin = durasiAktif / 60;
                            waitingFinalDegrada = true;

                            if (finalDegradaMin != lastQueuedDegradaMin) {
                                if (queueDegradaMeasurement(aktif->id, finalDegradaMin)) {
                                    lastQueuedDegradaMin = finalDegradaMin;
                                } else {
                                    waitingFinalDegrada = false; /* retry loop berikutnya */
                                }
                            }
                        }

                        if (waitingFinalDegrada &&
                            !wasDegradaSent(aktif->id, finalDegradaMin, currentTime)) {
                            if(xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                currentRemainingTime = 0;
                                xSemaphoreGive(mutexSystemState);
                            }
                            vTaskDelay(pdMS_TO_TICKS(100));
                            continue;
                        }
                    }

                    currentStep++;
                    if (currentStep < currentSchedules.count) {
                        activateStep(currentSchedules,
                                     currentStep,
                                     scheduleStartTime,
                                     nextDegradaMin,
                                     lastQueuedDegradaMin,
                                     spektroTriggered,
                                     waitingFinalDegrada,
                                     finalDegradaMin);
                    } else {
                        finishExecution();
                    }
                } else {
                    if(xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                        currentRemainingTime = durasiAktif - (int)elapsedSec;
                        xSemaphoreGive(mutexSystemState);
                    }
                }
            }
        }

        // Timer berjalan sebagai logic heartbeat/state switcher
        vTaskDelay(pdMS_TO_TICKS(100)); // Cek interval per 100ms cepat
    }
}
