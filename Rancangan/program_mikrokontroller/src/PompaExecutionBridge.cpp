#include <Arduino.h>
#include <stdint.h>
#include <string.h>
#include "SharedData.h"
#include "Config.h"

#define AERATOR_PWM_CHANNEL 8
#define AERATOR_PWM_FREQ_HZ 5000
#define AERATOR_PWM_RESOLUTION 8

extern "C" {

void pompaExecutionHardwareInit(void)
{
    ledcSetup(AERATOR_PWM_CHANNEL, AERATOR_PWM_FREQ_HZ, AERATOR_PWM_RESOLUTION);
    ledcAttachPin(GPIO_PWM_AERATOR, AERATOR_PWM_CHANNEL);
    ledcWrite(AERATOR_PWM_CHANNEL, 0);
}

int pompaIsExecutionActive(int *airflowLevel)
{
    int isExecution = 0;
    int level = 0;

    if (xSemaphoreTake(mutexSystemState, pdMS_TO_TICKS(1)) == pdTRUE) {
        isExecution = (currentSystemState == STATE_EKSEKUSI) ? 1 : 0;
        level = currentActiveParam.airflow_level;
        xSemaphoreGive(mutexSystemState);
    }

    if (airflowLevel != NULL) {
        *airflowLevel = level;
    }
    return isExecution;
}

void pompaSetAeratorDuty(uint32_t duty)
{
    if (duty > 255U) {
        duty = 255U;
    }
    ledcWrite(AERATOR_PWM_CHANNEL, duty);
}

/* Pompa kuvet hanya perlu menyala saat step aktif memang membutuhkan
 * spektrofotometer: step "spektro" (scan penuh) atau step dengan opsi
 * ukur_degradasi aktif (pengukuran degradasi tiap 5 menit). */
int pompaIsKuvetNeeded(void)
{
    int needed = 0;

    if (xSemaphoreTake(mutexSystemState, pdMS_TO_TICKS(1)) == pdTRUE) {
        needed = currentActiveParam.ukur_degradasi ||
                 (strcmp(currentActiveParam.reaksi, "spektro") == 0);
        xSemaphoreGive(mutexSystemState);
    }

    return needed;
}

}
