// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"

// ------------------------------------------------------------------------------------------------
// [PERINGATAN KESELAMATAN SANGAT PENTING - BACA SEBELUM MENGUBAH KODE INI]
// ------------------------------------------------------------------------------------------------
// Task ini membaca sensor Arus (Current Sensor) untuk memantau proses elektrofenton.
// KESELAMATAN PROGRAMMER & USER ADALAH YANG UTAMA! 
// Bila ada error logika pada pembacaan arus ini, sistem dapat merespons dengan salah, 
// misalnya: gagal mendeteksi arus lebih (overcurrent), atau mempertahankan arus tinggi 
// yang dapat menyebabkan OVERHEATING, KORSLETING, bahkan KEBAKARAN/LEDAKAN pada sistem alat elektrofenton.
// RESIKO: KORBAN JIWA!
// ------------------------------------------------------------------------------------------------

void taskBacaSensorArus(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Sensor Arus memulai...");

    pinMode(GPIO_PWM_FAN, OUTPUT);
    dacWrite(GPIO_DAC_ARUS, 0); // Pastikan sumber arus mati saat boot

    Serial.println("[Ready] Sensor Arus Siap!");

    // Tracking ramp linear arus: reset tiap kali step "ef" baru dimulai (id berubah),
    // agar lonjakan langsung ke arus target tidak mematikan alat (overcurrent trip).
    bool efActive = false;
    int efActiveId = -1;
    uint32_t efStartMs = 0;

    // ==== LOOPING TASK ====
    for(;;) {
        SystemState currentStateCopy;
        ScheduleParam paramCopy;

        // BACA STATE SYSTEM DENGAN MUTEX SECARA AMAN!
        if(xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
            currentStateCopy = currentSystemState;
            paramCopy = currentActiveParam;
            xSemaphoreGive(mutexSystemState);
        } else {
            currentStateCopy = STATE_IDLE;
        }

        // ==== KONTROL SUMBER ARUS ====
        if (currentStateCopy == STATE_EKSEKUSI && strcmp(paramCopy.reaksi, "ef") == 0) {
            if (!efActive || efActiveId != paramCopy.id) {
                efActive = true;
                efActiveId = paramCopy.id;
                efStartMs = millis();
            }

            // Ramp linear: 0 mA saat mulai -> arus target penuh di detik ke-30 (ARUS_RAMP_MS)
            uint32_t elapsedMs = millis() - efStartMs;
            float rampFactor = (float)elapsedMs / (float)ARUS_RAMP_MS;
            if (rampFactor > 1.0f) rampFactor = 1.0f;
            if (rampFactor < 0.0f) rampFactor = 0.0f;
            float arusTarget_mA = paramCopy.arus * rampFactor;

            // Konversi target arus: mA -> A -> tegangan DAC
            // Persamaan: Y (A) = 0.063 * X (V) + 5.33e-3
            // Invers   : X (V) = (Y - 5.33e-3) / 0.063
            float arusTarget_A = arusTarget_mA / 1000.0f;
            float targetVoltage = (arusTarget_A - 0.00533f) / 0.063f;

            // Clamp ke rentang DAC yang valid [0, 3.3V]
            if (targetVoltage < 0.0f) targetVoltage = 0.0f;
            if (targetVoltage > 3.3f) targetVoltage = 3.3f;

            int dacValue = (int)((targetVoltage / 3.3f) * 255);
            dacWrite(GPIO_DAC_ARUS, dacValue);
            //Serial.print("Target Arus: ");Serial.print(arusTarget_mA);
            //Serial.print(" mA -> DAC Voltage: ");Serial.print(targetVoltage, 3); Serial.print(" V -> DAC Value: "); Serial.println(dacValue);
            // Fan aktif hanya jika target arus > 0
            digitalWrite(GPIO_PWM_FAN, paramCopy.arus > 0 ? HIGH : LOW);
        } else {
            // Safety: di luar reaksi EF, matikan sumber arus dan fan
            dacWrite(GPIO_DAC_ARUS, 0);
            digitalWrite(GPIO_PWM_FAN, LOW);
            efActive = false;
            efActiveId = -1;
        }

        // ==== BACA SENSOR ARUS (ADS1115 A0, GAIN_SIXTEEN) ====
        // ADS1115 sudah diinisialisasi oleh taskBacaPH
        int16_t rawAdc = 0;
        float arusA = 0.0f;

        if (xSemaphoreTake(mutexI2C, portMAX_DELAY) == pdTRUE) {
            ads.setGain(GAIN_SIXTEEN); // +/- 0.256V, presisi untuk shunt
            rawAdc = ads.readADC_SingleEnded(0);
            arusA = ads.computeVolts(rawAdc); // computeVolts() = arus dalam A
            xSemaphoreGive(mutexI2C);
        }

        // Konversi ke mA untuk dikirim ke UI
        float arusMa = arusA * 1000.0f;

        // Tulis nilai ke Queue (overwrite: selalu simpan nilai terbaru)
        xQueueOverwrite(queueSensorArus, &arusMa);

        // DELAY KEAMANAN: Jeda CPU agar Watchdog tidak mereset ESP32.
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
