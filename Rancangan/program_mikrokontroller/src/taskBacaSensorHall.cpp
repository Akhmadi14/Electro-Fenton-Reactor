// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"

// ------------------------------------------------------------------------------------------------
// [PERINGATAN KESELAMATAN SANGAT PENTING - BACA SEBELUM MENGUBAH KODE INI]
// ------------------------------------------------------------------------------------------------
// Task ini membaca sensor Hall untuk mendapatkan data kecepatan rotasi (RPM) motor stirrer.
// KESELAMATAN PROGRAMMER & USER ADALAH YANG UTAMA! 
// Gangguan / error pada algoritma RPM ini dapat berujung pada respon controller yang tak sadar
// meningkatkan sinyal PWM motor stirrer hingga maksimum (Runaway Motor). Motor rotasi yang sangat 
// cepat tanpa deteksi rem dapat MEMBUAT CAIRAN KIMIA TERCIPRAT KELUAR ATAU VIBRASI MERUSAK TABUNG.
// RESIKO: KORBAN JIWA AKIBAT CAIRAN BERACUN ATAU PECAHAN KACA!
// ------------------------------------------------------------------------------------------------

static const unsigned long RPM_TIMEOUT_US = 500000UL; // 500 ms

static volatile unsigned long g_lastPulse_us = 0;  // waktu rising edge terakhir (us)
static volatile unsigned long g_period_us = 0;     // periode antar 2 pulsa terakhir (us)
static portMUX_TYPE g_hallMux = portMUX_INITIALIZER_UNLOCKED;

static void IRAM_ATTR hallISR() {
    unsigned long now = micros();
    portENTER_CRITICAL_ISR(&g_hallMux);
    if (g_lastPulse_us != 0) {
        g_period_us = now - g_lastPulse_us;
    }
    g_lastPulse_us = now;
    portEXIT_CRITICAL_ISR(&g_hallMux);
}

void taskBacaSensorHall(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Sensor Hall RPM memulai...");

    pinMode(GPIO_HALL_STIRRER, INPUT);
    attachInterrupt(digitalPinToInterrupt(GPIO_HALL_STIRRER), hallISR, RISING);

    Serial.println("[Ready] Sensor Hall Siap!");

    const TickType_t sampleTicks = pdMS_TO_TICKS(PERIOD_STIRRER_CONTROL_MS);
    TickType_t lastWakeTime = xTaskGetTickCount();
    bool hadPulse = false;

    // ==== LOOPING TASK ====
    for(;;) {
        SystemState currentStateCopy = STATE_IDLE;
        int targetRpm = 0;

        if (xSemaphoreTake(mutexSystemState, pdMS_TO_TICKS(1)) == pdTRUE) {
            currentStateCopy = currentSystemState;
            targetRpm = currentActiveParam.rpm;
            xSemaphoreGive(mutexSystemState);
        }

        if (targetRpm < 0) {
            targetRpm = 0;
        }

        bool inControl = (currentStateCopy == STATE_EKSEKUSI) && (targetRpm > 0);

        unsigned long period = 0;
        unsigned long lastPulse = 0;
        portENTER_CRITICAL(&g_hallMux);
        period = g_period_us;
        lastPulse = g_lastPulse_us;
        portEXIT_CRITICAL(&g_hallMux);

        unsigned long now_us = micros();
        bool motorStopped = (lastPulse == 0) || ((now_us - lastPulse) > RPM_TIMEOUT_US);

        float rpm = 0.0f;
        if (!motorStopped && period > 0) {
            rpm = 60000000.0f / (float)period;
        }
        if (rpm > 3000.0f) {
            rpm = 0.0f;
        }

        if (!inControl) {
            hadPulse = false;
            portENTER_CRITICAL(&g_hallMux);
            g_lastPulse_us = 0;
            g_period_us = 0;
            portEXIT_CRITICAL(&g_hallMux);
            rpm = 0.0f;
        } else if (lastPulse != 0) {
            hadPulse = true;
        }

        //int timeoutFlag = 0;
        //if (inControl && hadPulse) {
        //    if ((now_us - lastPulse) > RPM_TIMEOUT_US) {
        //        timeoutFlag = 1;
        //    }
        //}

        //if (xSemaphoreTake(mutexHallData, 0) == pdTRUE) {
          //  hallTimeoutFlag = timeoutFlag;
          //  xSemaphoreGive(mutexHallData);
        //}

        xQueueOverwrite(queueSensorRPM, &rpm);

        vTaskDelayUntil(&lastWakeTime, sampleTicks);
    }
}
