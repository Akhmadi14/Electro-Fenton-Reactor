/*
 * taskBacaSpektrum.c
 *
 * Task FreeRTOS untuk membaca data spektrofotometer.
 * Menerima perintah via queueSpektroCmd dari taskTimer.
 *
 * Dua mode operasi:
 *   SPEKTRO_CMD_DEGRADASI — ukur absorbansi pada λ tertentu
 *                           (1s prepare + scan hardware + 1s settle)
 *                           Hasilnya disimpan di lastDegradaPoint
 *                           → taskKomunikasi stream: "update_degradasi":{t_menit, absorbansi}
 *
 *   SPEKTRO_CMD_SPEKTRUM  — scan penuh 400–800nm
 *                           (1s prepare + scan hardware + 1s settle)
 *                           Hasilnya disimpan di lastSpektrum.data[]
 *                           → taskKomunikasi stream: {"reaksi":"spektro","id":X,"spektrum":[...]}
 *
 * ======  PANDUAN INTEGRASI HARDWARE  ======
 * Fungsi helper di bawah menjalankan alur hardware:
 *   1. Aktifkan sumber cahaya spektrofotometer (GPIO atau I2C)
 *   2. Atur panjang gelombang (motor stepper / filter wheel)
 *   3. Trigger ADC / I2C sensor intensitas
 *   4. Baca nilai, konversi ke absorbansi: A = -log10(I/I0)
 *      di mana I0 adalah referensi kalibrasi yang disimpan saat
 *      runKalibrasiSpektro() berjalan.
 * ==========================================
 */

#include "SpektroShared.h"
#include "Config.h"
#include <stdio.h>

extern SemaphoreHandle_t mutexSystemState;

static float readPeakAbsorbansi(void)
{
    if (!spektroScanAbsorbance(NULL, 0)) {
        return 0.0f;
    }
    return spektroPeakAbsorbance();
}

static void readSpektrum(void)
{
    spektroScanAbsorbance(lastSpektrum.data, SPEKTRUM_POINTS);
}

/* ====================================================================
 * Entry point FreeRTOS task
 * ==================================================================== */
void taskBacaSpektrum(void *pvParameters)
{
    (void)pvParameters;
    printf("[Init] Task Baca Spektrum Siap!\n");
    spektroHardwareInit();
    spektroSetHalogenDuty(0);

    SpektroCmd cmd;

    for (;;) {
        /* Blok hingga ada perintah (max 500ms lalu cek ulang) */
        if (xQueueReceive(queueSpektroCmd, &cmd, pdMS_TO_TICKS(500)) != pdTRUE) {
            continue;
        }

        if (cmd.cmdType == SPEKTRO_CMD_DEGRADASI) {
            /* --- Ukur puncak absorbansi untuk grafik degradasi --- */
            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                degradaSync.busy     = 1;
                degradaSync.produced = 0;
                degradaSync.sent     = 0;
                degradaSync.reaksiId = cmd.reaksiId;
                degradaSync.t_menit  = cmd.t_menit;
                degradaSync.sentAtMs = 0;
                xSemaphoreGive(mutexSystemState);
            }

            /* Prefill kuvet: tunggu fluida mengalir ke kuvet sebelum ukur pertama.
             * Hanya pengukuran pertama (t_menit == 0) yang perlu prefill.
             * Ubah KUVET_PREFILL_FIRST_DEGRADA_MS di Config.h untuk menyesuaikan. */
            if (cmd.t_menit == 0) {
                printf("[spektro] Prefill kuvet %d ms (ukur pertama)...\n",
                       KUVET_PREFILL_FIRST_DEGRADA_MS);
                vTaskDelay(pdMS_TO_TICKS(KUVET_PREFILL_FIRST_DEGRADA_MS));
            }

            spektroSetHalogenDuty(SPEKTRO_HALOGEN_DUTY_ON);
            vTaskDelay(pdMS_TO_TICKS(SPEKTRO_PREPARE_DELAY_MS));
            float abs = readPeakAbsorbansi();
            vTaskDelay(pdMS_TO_TICKS(SPEKTRO_SETTLE_DELAY_MS));
            spektroSetHalogenDuty(0);

            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                lastDegradaPoint.reaksiId   = cmd.reaksiId;
                lastDegradaPoint.t_menit    = cmd.t_menit;
                lastDegradaPoint.absorbansi = abs;
                lastDegradaPoint.ready      = 1;
                degradaSync.produced        = 1;
                xSemaphoreGive(mutexSystemState);
            }
        }
        else if (cmd.cmdType == SPEKTRO_CMD_SPEKTRUM) {
            /* --- Scan penuh 400-800nm --- */
            /* Tandai belum siap sebelum mulai scan */
            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                spektrumSync.busy     = 1;
                spektrumSync.produced = 0;
                spektrumSync.sent     = 0;
                spektrumSync.reaksiId = cmd.reaksiId;
                spektrumSync.sentAtMs = 0;
                lastSpektrum.id       = cmd.reaksiId;
                lastSpektrum.ready    = 0;
                xSemaphoreGive(mutexSystemState);
            }

            /* Prefill kuvet: fluida dari reaktor perlu waktu untuk mencapai kuvet.
             * Scan spektrum SELALU didahului prefill karena kuvet bisa kosong.
             * Ubah KUVET_PREFILL_SPEKTRUM_MS di Config.h untuk menyesuaikan. */
            printf("[spektro] Prefill kuvet %d ms (scan spektrum)...\n",
                   KUVET_PREFILL_SPEKTRUM_MS);
            vTaskDelay(pdMS_TO_TICKS(KUVET_PREFILL_SPEKTRUM_MS));

            spektroSetHalogenDuty(SPEKTRO_HALOGEN_DUTY_ON);
            vTaskDelay(pdMS_TO_TICKS(SPEKTRO_PREPARE_DELAY_MS));
            readSpektrum();
            vTaskDelay(pdMS_TO_TICKS(SPEKTRO_SETTLE_DELAY_MS));
            spektroSetHalogenDuty(0);

            if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
                lastSpektrum.ready    = 1;
                spektrumSync.produced = 1;
                xSemaphoreGive(mutexSystemState);
            }
        }
    }
}
