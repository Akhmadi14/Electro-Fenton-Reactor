// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"

static const float KP = 0.005f;
static const float KI = 0.046f;
static const float INTEGRAL_LIMIT = 5000.0f;
static const int PWM_MIN = 0;
static const int PWM_MAX = 250;
static const float SAMPLE_INTERVAL_S = 0.02f;

void taskKontrolStirrer(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Task Kontrol Stirrer Siap!");
    
    // Setup PWM untuk Stirrer menggunakan pin konfigurasi
    // Pada ESP32, kita gunakan ledcSetup (channel 0-15)
    // ledcSetup(channel, freq, resolution_bits)
    const int pwmChannel = 0;
    const int pwmFreq = 5000;
    const int pwmResolution = 8; // 0-255
    
    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(GPIO_PWM_STIRRER, pwmChannel);
    ledcWrite(pwmChannel, 0);

    float currentTemp = 0.0f;
    int currentPWM = -1;
    float integral = 0.0f;
    float prev_error = 0.0f;

    const TickType_t sampleTicks = pdMS_TO_TICKS(PERIOD_STIRRER_CONTROL_MS);
    TickType_t lastWakeTime = xTaskGetTickCount();
    float rpmMeasured = 0.0f;
    // ==== LOOPING TASK ====
    for(;;) {
        if (xQueueReceive(queueSuhuForStirrer, &currentTemp, 0) == pdTRUE) {
            // Data suhu tidak digunakan untuk kontrol stirrer, hanya membersihkan queue.
        }
        
        int targetRpm = 0;
        if (xSemaphoreTake(mutexSystemState, pdMS_TO_TICKS(1)) == pdTRUE) {
            if (currentSystemState == STATE_EKSEKUSI) {
                targetRpm = currentActiveParam.rpm;
            }
            xSemaphoreGive(mutexSystemState);
        }

        if (targetRpm < 0) {
            targetRpm = 0;
        }

        
        if (xQueuePeek(queueSensorRPM, &rpmMeasured, 0) != pdTRUE) {
            rpmMeasured = 0.0f;
        }

        //int hallTimeout = 0;
        //if (xSemaphoreTake(mutexHallData, 0) == pdTRUE) {
        //    hallTimeout = hallTimeoutFlag;
        //    xSemaphoreGive(mutexHallData);
        //}

        float pwm_cmd_f = 0.0f;
        if (targetRpm > 0 ) { 
            float error = (float)targetRpm - rpmMeasured;
            integral += error * SAMPLE_INTERVAL_S;
            if (integral > INTEGRAL_LIMIT) {
                integral = INTEGRAL_LIMIT;
            } else if (integral < -INTEGRAL_LIMIT) {
                integral = -INTEGRAL_LIMIT;
            }
            prev_error = error;
            pwm_cmd_f = (KP * error) + (KI * integral);
        } else {
            integral = 0.0f;
            prev_error = 0.0f;
            pwm_cmd_f = 0.0f;
        }

        int pwm_cmd = (int)pwm_cmd_f;
        if (pwm_cmd < PWM_MIN) {
            pwm_cmd = PWM_MIN;
        } else if (pwm_cmd > PWM_MAX) {
            pwm_cmd = PWM_MAX;
        }

        if (pwm_cmd != currentPWM) {
            currentPWM = pwm_cmd;
            ledcWrite(pwmChannel, currentPWM);
        }

        // Debug opsional
        //Serial.printf("[Stirrer] Target RPM: %d -> Measured RPM: %.2f -> PWM: %d , PWMCMD: %.2f\n", targetRpm, rpmMeasured, currentPWM, pwm_cmd_f);

        vTaskDelayUntil(&lastWakeTime, sampleTicks);
    }
}
