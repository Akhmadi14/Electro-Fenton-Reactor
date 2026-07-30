// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"
#include "PompaShared.h"
#include "SpektroShared.h"
#include <ArduinoJson.h>

void taskKomunikasi(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Task Komunikasi Siap!");
    
    // Tambahan flag/variabel untuk sensor pembacaan
    float lastPH = 0.0f, lastSuhu = 0.0f, lastArus = 0.0f, lastRPM = 0.0f;
    GasStatus lastGas = { "CO2 not detected" };
    String cmdBuffer = "";
    unsigned long lastStreamTime = 0;
    unsigned long serialQuietUntil = 0;

    // ==== LOOPING TASK ====
    for(;;) {
        // --- 1. MEMBACA COMMAND DARI UI (SERIAL MONITOR) ---
        while(Serial.available() > 0) {
            char c = Serial.read();
            if(c == '\n' || c == '\r') {
                if(cmdBuffer.length() > 0) {
                    cmdBuffer.trim();

                    // --- Ambil state saat ini untuk guard ---
                    SystemState cmdState = STATE_IDLE;
                    if (xSemaphoreTake(mutexSystemState, 10) == pdTRUE) {
                        cmdState = currentSystemState;
                        xSemaphoreGive(mutexSystemState);
                    }

                    if (cmdBuffer.equals(CMD_STOP)) {
                        if (cmdState == STATE_EKSEKUSI) {
                            int forceCmd = 1;
                            xQueueSend(queueForceStop, &forceCmd, 0);
                            Serial.println("ACK:FORCE_STOP");
                        } else {
                            Serial.println("{\"NACK\":\"Force stop hanya saat eksekusi\"}");
                        }
                    }
                    else if (cmdBuffer.equals("ACK_DONE")) {
                        if (cmdState == STATE_INISIALISASI_EKSEKUSI && initDoneFlag) {
                            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                currentSystemState = STATE_IDLE;
                                initDoneFlag = 0;
                                InisialisasiProgress emptyProg = {0, 0, 0, 0, -1};
                                currentInitProgress = emptyProg;
                                xSemaphoreGive(mutexSystemState);
                            }
                        }
                    }
                    else if (cmdBuffer.equals("ACK_FLUSHINGDONE")) {
                        if (cmdState == STATE_FLUSHING && flushDoneFlag) {
                            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                currentSystemState = STATE_INISIALISASI_INPUT;
                                flushDoneFlag = 0;
                                memset(&currentFlushProgress, 0, sizeof(FlushingProgress));
                                xSemaphoreGive(mutexSystemState);
                            }
                            Serial.println("ACK:INISIALISASI_INPUT");
                        }
                    }
                    else if (cmdBuffer.startsWith("{")) {
                        JsonDocument doc;
                        DeserializationError error = deserializeJson(doc, cmdBuffer);

                        if (!error) {
                            /* --- START JADWAL (hanya dari STATE_IDLE) --- */
                            if (doc["status"] == "start" && doc["jadwal"].is<JsonArray>()) {
                                if (cmdState != STATE_IDLE) {
                                    Serial.println("{\"NACK\":\"Sistem tidak idle\"}");
                                } else {
                                    JsonArray jadwal = doc["jadwal"].as<JsonArray>();
                                    ScheduleArray schedArray;
                                    schedArray.count = 0;

                                    if (jadwal.size() > MAX_SCHEDULES) {
                                        Serial.print("{\"NACK\":\"Maksimum ");
                                        Serial.print(MAX_SCHEDULES);
                                        Serial.println(" jadwal per eksekusi\"}");
                                    } else {
                                        for (JsonObject item : jadwal) {
                                            strlcpy(schedArray.schedules[schedArray.count].reaksi,
                                                    item["reaksi"] | "",
                                                    sizeof(schedArray.schedules[0].reaksi));
                                            schedArray.schedules[schedArray.count].suhu   = item["suhu"] | 0.0;
                                            schedArray.schedules[schedArray.count].rpm    = item["rpm"] | 0;
                                            schedArray.schedules[schedArray.count].durasi = item["durasi"] | 0;
                                            schedArray.schedules[schedArray.count].arus   = item["arus"] | 0;
                                            schedArray.schedules[schedArray.count].airflow_level = item["airflow_level"] | 0;
                                            schedArray.schedules[schedArray.count].ukur_degradasi = item["ukur_degradasi"] | false;
                                            /* id: gunakan nilai dari UI atau auto-assign sequential */
                                            schedArray.schedules[schedArray.count].id = item["id"] | (schedArray.count + 1);
                                            schedArray.count++;
                                        }
                                    }

                                    if (schedArray.count > 0) {
                                        if (xQueueSend(queueScheduleParam, &schedArray, 0) == pdPASS) {
                                            Serial.println("ACK_START");
                                        } else {
                                            Serial.println("{\"NACK\":\"Queue Jadwal Penuh\"}");
                                        }
                                    } else if (jadwal.size() <= MAX_SCHEDULES) {
                                        Serial.println("{\"NACK\":\"Jadwal Kosong/Tidak Valid\"}");
                                    }
                                }
                            }
                            /* --- INISIALISASI (hanya dari STATE_INISIALISASI_INPUT) --- */
                            else if (doc["status"] == "inisialisasi") {
                                if (cmdState != STATE_INISIALISASI_INPUT) {
                                    Serial.println("{\"NACK\":\"Bukan state inisialisasi input\"}");
                                } else {
                                    InisialisasiConfig cfg;
                                    cfg.vol       = doc["vol"] | 0;
                                    cfg.ph        = doc["ph"] | -1.0f;
                                    cfg.fe        = doc["fe"] | -1.0f;
                                        strlcpy(cfg.fe_type,
                                            doc["fe_type"] | "ml",
                                            sizeof(cfg.fe_type));
                                    cfg.kalibrasi  = doc["kalibrasi"]  | 0;
                                    cfg.prefilling = doc["prefilling"] | -1;

                                    if (cfg.vol < 50 || cfg.vol > 250) {
                                        Serial.println("{\"NACK\":\"Volume harus 50-250 mL\"}");
                                    } 
                                    //else if (cfg.ph >= 0 && cfg.ph < 250.0f) {
                                    //    Serial.println("{\"NACK\":\"H2SO4 maksimum 250\"}");
                                    //} 
                                    else {
                                        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                            currentSystemState = STATE_INISIALISASI_EKSEKUSI;
                                            xSemaphoreGive(mutexSystemState);
                                        }
                                        if (xQueueSend(queueInisialisasiCmd, &cfg, 0) == pdPASS) {
                                            Serial.println("ACK_START");
                                        } else {
                                            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                                currentSystemState = STATE_INISIALISASI_INPUT;
                                                xSemaphoreGive(mutexSystemState);
                                            }
                                            Serial.println("{\"NACK\":\"Queue pompa penuh\"}");
                                        }
                                    }
                                }
                            }
                            /* --- FLUSHING (hanya dari STATE_IDLE) --- */
                            else if (doc["status"] == "flushing") {
                                if (cmdState != STATE_IDLE) {
                                    Serial.println("{\"NACK\":\"Sistem tidak idle\"}");
                                } else {
                                    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                        currentSystemState = STATE_FLUSHING;
                                        xSemaphoreGive(mutexSystemState);
                                    }
                                    int flushCmd = 1;
                                    if (xQueueSend(queueFlushingCmd, &flushCmd, 0) == pdPASS) {
                                        Serial.println("ACK_FLUSHING");
                                    } else {
                                        if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                                            currentSystemState = STATE_IDLE;
                                            xSemaphoreGive(mutexSystemState);
                                        }
                                        Serial.println("{\"NACK\":\"Queue flushing penuh\"}");
                                    }
                                }
                            }
                        } else {
                            Serial.print("JSON Parse Error: ");
                            Serial.println(error.c_str());
                        }
                    }

                    cmdBuffer = ""; // reset buffer
                }
            } else {
                cmdBuffer += c;
            }
        }

        // --- 2. MENDAPATKAN DATA SENSOR TERKINI ---
        xQueuePeek(queueSensorPH, &lastPH, 0);
        xQueuePeek(queueSensorSuhu, &lastSuhu, 0);
        xQueuePeek(queueSensorGas, &lastGas, 0);
        xQueuePeek(queueSensorArus, &lastArus, 0);
        xQueuePeek(queueSensorRPM, &lastRPM, 0);

        // --- 2b. STREAMING SPEKTRUM (di luar timer 1s, segera saat siap) ---
        // Gunakan static buffer agar tidak memakan stack (data 128 float ≈ 512 B)
        {
            static SpektrumResult spektBuf;
            int hasSpektrum = 0;
            if (xSemaphoreTake(mutexSystemState, 5) == pdTRUE) {
                if (lastSpektrum.ready) {
                    spektBuf        = lastSpektrum;
                    lastSpektrum.ready = 0; // konsumsi
                    hasSpektrum     = 1;
                }
                xSemaphoreGive(mutexSystemState);
            }
            if (hasSpektrum) {
                /* Stream manual JSON untuk menghindari heap besar di ArduinoJson */
                Serial.print("{\"reaksi\":\"spektro\",\"id\":");
                Serial.print(spektBuf.id);
                Serial.print(",\"spektrum\":[");
                for (int i = 0; i < SPEKTRUM_POINTS; i++) {
                    if (i > 0) Serial.write(',');
                    Serial.print(spektBuf.data[i], 4);
                }
                Serial.println("],\"State\":\"Eksekusi\"}");

                /* Tandai payload panjang benar-benar sudah keluar ke UART,
                 * lalu beri jeda 2 detik agar UI punya waktu parsing. */
                unsigned long sentAt = millis();
                if (xSemaphoreTake(mutexSystemState, 10) == pdTRUE) {
                    spektrumSync.sent     = 1;
                    spektrumSync.busy     = 0;
                    spektrumSync.sentAtMs = sentAt;
                    xSemaphoreGive(mutexSystemState);
                }
                serialQuietUntil = sentAt + SPEKTRUM_SERIAL_GUARD_MS;
                lastStreamTime = sentAt;
            }
        }

        // --- 3. STREAMING DATA KE UI SECARA PERIODIK ---
        unsigned long currentMillis = millis();
        if (currentMillis >= serialQuietUntil &&
            currentMillis - lastStreamTime >= PERIOD_STREAM) {
            lastStreamTime = currentMillis;

            SystemState stateCpy;
            ScheduleParam localParam;
            int localTime = 0;
            InisialisasiProgress localInitProg = {0, 0, 0, 0, -1};
            FlushingProgress     localFlushProg = {0};
            int localInitDone  = 0;
            int localFlushDone = 0;
            DegradasiPoint localDegrada = {0, 0, 0.0f, 0};
            int hasDegrada = 0;
            bool shouldSerialize = true;

            if(xSemaphoreTake(mutexSystemState, 10) == pdTRUE) {
                stateCpy = currentSystemState;
                if (stateCpy == STATE_EKSEKUSI) {
                    localParam = currentActiveParam;
                    localTime  = currentRemainingTime;
                    /* Ambil data degradasi jika ada (konsumsi sekaligus) */
                    if (lastDegradaPoint.ready) {
                        localDegrada             = lastDegradaPoint;
                        lastDegradaPoint.ready   = 0;
                        hasDegrada               = 1;
                    }
                } else if (stateCpy == STATE_INISIALISASI_EKSEKUSI) {
                    localInitProg = currentInitProgress;
                    localInitDone = initDoneFlag;
                } else if (stateCpy == STATE_FLUSHING) {
                    localFlushProg = currentFlushProgress;
                    localFlushDone = flushDoneFlag;
                }
                xSemaphoreGive(mutexSystemState);
            } else {
                stateCpy = STATE_IDLE;
            }

            JsonDocument docOut;

            if (stateCpy == STATE_INISIALISASI_EKSEKUSI) {
                docOut["status"]    = localInitDone ? "done" : "ongoing";
                docOut["sampel"]    = localInitProg.sampelPercent;
                docOut["h2so4"]     = localInitProg.h2so4Status;
                docOut["feso4"]     = localInitProg.feso4Status;
                docOut["kalibrasi"] = localInitProg.kalibrasiStatus;
                docOut["prefill"]   = localInitProg.prefillStatus;
            }
            else if (stateCpy == STATE_FLUSHING) {
                docOut["status"]   = localFlushDone ? "flushing_done" : "flushing";
                docOut["progress"] = localFlushProg.progressPercent;
            }
            else if (stateCpy == STATE_EKSEKUSI) {
                /* Step "spektro" memakai payload khusus terpisah.
                 * Jangan campur heartbeat periodik dengan payload spektrum panjang. */
                if (strcmp(localParam.reaksi, "spektro") == 0) {
                    shouldSerialize = false;
                } else {
                    docOut["reaksi"] = localParam.reaksi;
                    docOut["id"]     = localParam.id;

                    JsonObject paramObj = docOut["parameter"].to<JsonObject>();
                    paramObj["suhu"]           = (double)lastSuhu;
                    paramObj["rpm"]            = (double)lastRPM;
                    paramObj["ukur_degradasi"] = localParam.ukur_degradasi;
                    paramObj["waktu_tersisa"]  = localTime;

                    if (strcmp(localParam.reaksi, "ef") == 0) {
                        paramObj["pH"]            = (double)lastPH;
                        paramObj["gas"]           = lastGas.value;
                        paramObj["arus"]          = (double)lastArus;
                        paramObj["airflow_level"] = localParam.airflow_level;
                    }

                    /* Sertakan update_degradasi bila ada hasil pengukuran baru */
                    if (hasDegrada) {
                        JsonObject deg = docOut["update_degradasi"].to<JsonObject>();
                        deg["t_menit"]    = localDegrada.t_menit;
                        deg["absorbansi"] = (double)localDegrada.absorbansi;
                    }

                    docOut["State"] = "Eksekusi";
                }
            }
            else if (stateCpy == STATE_INISIALISASI_INPUT) {
                docOut["State"] = "InisialisasiInput";
                docOut["pH"]    = (double)lastPH;
                docOut["suhu"]  = (double)lastSuhu;
                docOut["gas"]   = lastGas.value;
            }
            else {
                /* STATE_IDLE */
                docOut["State"] = "Idle";
                docOut["pH"]    = (double)lastPH;
                docOut["suhu"]  = (double)lastSuhu;
                docOut["gas"]   = lastGas.value;
            }

            if (shouldSerialize) {
                serializeJson(docOut, Serial);
                Serial.println();

                if (hasDegrada) {
                    unsigned long sentAt = millis();
                    if (xSemaphoreTake(mutexSystemState, 10) == pdTRUE) {
                        if (degradaSync.reaksiId == localDegrada.reaksiId &&
                            degradaSync.t_menit == localDegrada.t_menit) {
                            degradaSync.sent     = 1;
                            degradaSync.busy     = 0;
                            degradaSync.sentAtMs = sentAt;
                        }
                        xSemaphoreGive(mutexSystemState);
                    }
                }
            }
        }

        // DELAY KEAMANAN: Memastikan Watchdog Timer tidak terpicu (Fatal Error reset)
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
