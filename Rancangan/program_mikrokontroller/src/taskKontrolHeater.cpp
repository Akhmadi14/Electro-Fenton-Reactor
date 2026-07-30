// Header dihapus, prototipe dipindah ke SharedData.h
#include "SharedData.h"
#include "Config.h"

static const float KP = 6.977124f;
static const float KI = 0.0064603f;
static const float KD = 1302.39648f;
static const float SETPOINT_DIFF = 10.0f;
static const unsigned long REFRESH_RATE_MS = 1000UL;
static const unsigned long PID_REFRESH_MS = 60000UL;

void taskKontrolHeater(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Task Kontrol Heater Siap!");
    
    // Setup PWM untuk Heater dengan SSR
    // Channel beda dengan stirrer, kita pakai channel 1
    const int heaterPwmChannel = 1;
    const int heaterPwmFreq = 100; // SSR AC biasanya mending frekuensi rendah/slow PWM
    const int heaterPwmResolution = 8; // 0-255
    
    ledcSetup(heaterPwmChannel, heaterPwmFreq, heaterPwmResolution);
    ledcAttachPin(GPIO_PWM_HEATER, heaterPwmChannel);

    float currentTemp = 0.0f;
    float prevTemp = 0.0f;
    float setpoint = 0.0f;
    unsigned long prevTime = 0;
    uint8_t state = 0;
    bool initialPID = true;
    float PID_p = 0.0f;
    float PID_i = 0.0f;
    float PID_d = 0.0f;
    float PID_total = 0.0f;
    int currentPWM = 0;

    ledcWrite(heaterPwmChannel, 255);

    // ==== LOOPING TASK ====
    for(;;) {
        float tempSample = 0.0f;
        if (xQueueReceive(queueSuhuForHeater, &tempSample, 0) == pdTRUE) {
            currentTemp = tempSample;
        }

        if (xSemaphoreTake(mutexSystemState, pdMS_TO_TICKS(1)) == pdTRUE) {
            if (currentSystemState == STATE_EKSEKUSI) {
                setpoint = currentActiveParam.suhu;
            } else {
                setpoint = 0.0f;
            }
            xSemaphoreGive(mutexSystemState);
        }

        if (setpoint <= 0.0f) {
            ledcWrite(heaterPwmChannel, 255);
            currentPWM = 0;
            vTaskDelay(pdMS_TO_TICKS(PERIOD_KONTROL));
            continue;
        }

        unsigned long nowMs = millis();
        unsigned long elapsed = nowMs - prevTime;

        if (state == 0) {
            if (elapsed > REFRESH_RATE_MS) {
                if (currentTemp < (setpoint - SETPOINT_DIFF)) {
                    currentPWM = 255;
                    ledcWrite(heaterPwmChannel, 0);
                } else {
                    currentPWM = 0;
                    ledcWrite(heaterPwmChannel, 255);
                    state = 1;
                }
                prevTime = nowMs;
            }
        } else if (state == 1) {
            if (elapsed > PID_REFRESH_MS || initialPID) {
                if (initialPID) {
                    prevTemp = currentTemp;
                }
                initialPID = false;

                float nowPidError = setpoint - currentTemp;
                PID_p = KP * nowPidError;
                PID_d = -KD * ((currentTemp - prevTemp) / (PID_REFRESH_MS / 1000.0f));

                PID_i = PID_i + (KI * nowPidError);
                if (PID_i < 0.0f) {
                    PID_i = 0.0f;
                } else if (PID_i > 255.0f) {
                    PID_i = 255.0f;
                }

                PID_total = PID_p + PID_i + PID_d;
                if (PID_total < 0.0f) {
                    PID_total = 0.0f;
                } else if (PID_total < 4.0f) {
                    PID_total = 4.0f;
                }
                if (PID_total > 255.0f) {
                    PID_total = 255.0f;
                }

                currentPWM = (int)PID_total;
                ledcWrite(heaterPwmChannel, 255 - currentPWM);

                prevTime = nowMs;
                prevTemp = currentTemp;
            }
        } else {
            ledcWrite(heaterPwmChannel, 255);
            currentPWM = 0;
            if (elapsed > REFRESH_RATE_MS) {
                prevTime = nowMs;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(PERIOD_KONTROL));
    }
}
