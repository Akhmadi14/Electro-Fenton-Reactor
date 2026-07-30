// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"
#include <string.h>

void taskBacaGas(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    // [FIX] Race condition: taskBacaPH memanggil ads.begin() di awal task-nya.
    // Jika taskBacaGas langsung memanggil ads.readADC_SingleEnded() sebelum
    // ads.begin() selesai, internal I2CDevice pointer masih NULL → Guru Meditation.
    // Tunda 500 ms agar taskBacaPH punya waktu menyelesaikan inisialisasi ADS1115.
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.println("[Init] Sensor Gas (MQ-135 di ADS1115) Siap!");

    const TickType_t sampleTicks = pdMS_TO_TICKS(PERIOD_BACA_GAS);
    TickType_t lastWakeTime = xTaskGetTickCount();
    float prevMv = 0.0f;
    bool hasPrev = false;
    GasStatus gasStatus;
    strlcpy(gasStatus.value, "CO2 not detected", GAS_STATUS_LEN);

    // ==== LOOPING TASK ====
    for(;;) {
        int16_t rawAdc = 0;
        float gasMv = 0.0f;
        
        // Membaca nilai dari pin analog ADS1115 Channel A2, konversi ke miliVolt
        if(xSemaphoreTake(mutexI2C, portMAX_DELAY) == pdTRUE) {
            ads.setGain(GAIN_TWO);
            rawAdc = ads.readADC_SingleEnded(2);
            gasMv = ads.computeVolts(rawAdc) * 1000.0f; // Volt -> mV
            xSemaphoreGive(mutexI2C);
        }

        if (hasPrev) {
            float delta = gasMv - prevMv;
            printf("Gas Sensor: %.1f mV, Delta: %.1f mV\n", gasMv, delta);

            // Prioritas 1 (tertinggi): CO2 not detected — tegangan naik >= threshold
            if (delta >= GAS_DELTA_THRESHOLD_MV) {
                strlcpy(gasStatus.value, "CO2 not detected", GAS_STATUS_LEN);
            }
            // Prioritas 2: CO2 detected — dua kondisi OR:
            //   A) Delta turun >= threshold DAN tegangan sebelumnya di rentang 20–25 mV
            //   B) Tegangan saat ini di bawah 19 mV (absolut rendah)
            else if ((delta <= -GAS_DELTA_THRESHOLD_MV &&
                      prevMv >= GAS_RANGE_LOW_MV && prevMv <= GAS_RANGE_HIGH_MV) ||
                     (gasMv < GAS_ABS_LOW_MV)) {
                strlcpy(gasStatus.value, "CO2 detected", GAS_STATUS_LEN);
            }
            // Selain itu: pertahankan state sebelumnya (tidak ada perubahan)
        }

        prevMv = gasMv;
        hasPrev = true;

        xQueueOverwrite(queueSensorGas, &gasStatus);

        vTaskDelayUntil(&lastWakeTime, sampleTicks);
    }
}
