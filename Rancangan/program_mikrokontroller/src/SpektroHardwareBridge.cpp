#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <math.h>
#include "SharedData.h"
#include "Config.h"
#include "SpektroShared.h"

#define HALOGEN_PWM_CHANNEL 9
#define HALOGEN_PWM_FREQ_HZ 5000
#define HALOGEN_PWM_RESOLUTION 8

static bool spektroInitialized = false;

extern "C" {

uint16_t spektroValue[SPEKTRO_PIXEL_COUNT] = {0};
uint16_t spektroReference[SPEKTRO_PIXEL_COUNT] = {0};
float spektroAbsorbance[SPEKTRO_PIXEL_COUNT] = {0.0f};
int spektroHasReference = 0;
int spektroExposureUs = SPEKTRO_DEFAULT_EXPOSURE_US;
uint32_t spektroLastExposureUs = 0;

}

static int clampExposure(long exposureUs)
{
    if (exposureUs < SPEKTRO_AE_EXP_MIN) {
        return SPEKTRO_AE_EXP_MIN;
    }
    if (exposureUs > SPEKTRO_AE_EXP_MAX) {
        return SPEKTRO_AE_EXP_MAX;
    }
    return (int)exposureUs;
}

static void clearAbsorbance(void)
{
    for (int i = 0; i < SPEKTRO_PIXEL_COUNT; i++) {
        spektroAbsorbance[i] = 0.0f;
    }
}

static void fillInterpolatedOutput(float *outData, int outCount)
{
    if (outData == NULL || outCount <= 0) {
        return;
    }
    if (outCount == 1) {
        outData[0] = spektroAbsorbance[0];
        return;
    }

    for (int i = 0; i < outCount; i++) {
        float pos = ((float)i * (float)(SPEKTRO_PIXEL_COUNT - 1)) / (float)(outCount - 1);
        int left = (int)pos;
        if (left >= SPEKTRO_PIXEL_COUNT - 1) {
            outData[i] = spektroAbsorbance[SPEKTRO_PIXEL_COUNT - 1];
        } else {
            float frac = pos - (float)left;
            float a = spektroAbsorbance[left];
            float b = spektroAbsorbance[left + 1];
            outData[i] = a + ((b - a) * frac);
        }
    }
}

static int captureCamera(void)
{
    spektroHardwareInit();

    if (xSemaphoreTake(mutexI2C, portMAX_DELAY) != pdTRUE) {
        return 0;
    }

    adsGain_t prevGain = ads.getGain();
    uint16_t prevRate = ads.getDataRate();

    ads.setGain(GAIN_TWO);
    ads.setDataRate(RATE_ADS1115_860SPS);

    digitalWrite(GPIO_SPEKTRO_CLOCK, LOW);
    digitalWrite(GPIO_SPEKTRO_SI, HIGH);
    digitalWrite(GPIO_SPEKTRO_CLOCK, HIGH);
    digitalWrite(GPIO_SPEKTRO_SI, LOW);
    digitalWrite(GPIO_SPEKTRO_CLOCK, LOW);

    for (int j = 0; j < 128; j++) {
        digitalWrite(GPIO_SPEKTRO_CLOCK, HIGH);
        digitalWrite(GPIO_SPEKTRO_CLOCK, LOW);
    }

    uint32_t startUs = micros();
    delayMicroseconds((uint32_t)spektroExposureUs);
    spektroLastExposureUs = micros() - startUs;

    digitalWrite(GPIO_SPEKTRO_SI, HIGH);
    digitalWrite(GPIO_SPEKTRO_CLOCK, HIGH);
    digitalWrite(GPIO_SPEKTRO_SI, LOW);
    digitalWrite(GPIO_SPEKTRO_CLOCK, LOW);

    for (int j = 0; j < 128; j++) {
        delayMicroseconds(20);
        int16_t raw = ads.readADC_SingleEnded(3);
        if (raw < 0) {
            raw = 0;
        }

        if (j >= SPEKTRO_PIXEL_START && j <= SPEKTRO_PIXEL_END) {
            /* Pembacaan terbalik: pixel pertama = 800nm, pixel terakhir = 400nm.
             * Simpan terbalik agar spektroValue[0] = 400nm. */
            int idx = SPEKTRO_PIXEL_COUNT - 1 - (j - SPEKTRO_PIXEL_START);
            spektroValue[idx] = (uint16_t)raw;
        }

        digitalWrite(GPIO_SPEKTRO_CLOCK, HIGH);
        digitalWrite(GPIO_SPEKTRO_CLOCK, LOW);
    }

    ads.setGain(prevGain);
    ads.setDataRate(prevRate);
    xSemaphoreGive(mutexI2C);
    return 1;
}

extern "C" {

void spektroHardwareInit(void)
{
    if (spektroInitialized) {
        return;
    }

    pinMode(GPIO_SPEKTRO_SI, OUTPUT);
    pinMode(GPIO_SPEKTRO_CLOCK, OUTPUT);
    digitalWrite(GPIO_SPEKTRO_CLOCK, LOW);
    digitalWrite(GPIO_SPEKTRO_SI, LOW);

    ledcSetup(HALOGEN_PWM_CHANNEL, HALOGEN_PWM_FREQ_HZ, HALOGEN_PWM_RESOLUTION);
    ledcAttachPin(GPIO_PWM_HALOGEN, HALOGEN_PWM_CHANNEL);
    ledcWrite(HALOGEN_PWM_CHANNEL, 0);

    spektroInitialized = true;
}

void spektroSetHalogenDuty(uint32_t duty)
{
    spektroHardwareInit();
    if (duty > 255U) {
        duty = 255U;
    }
    ledcWrite(HALOGEN_PWM_CHANNEL, duty);
}

int spektroAutoExposure(void)
{
    int converged = 0;

    for (int iter = 0; iter < SPEKTRO_AE_MAX_ITER; iter++) {
        if (!captureCamera()) {
            return 0;
        }

        int maxVal = 0;
        for (int i = 0; i < SPEKTRO_PIXEL_COUNT; i++) {
            if ((int)spektroValue[i] > maxVal) {
                maxVal = (int)spektroValue[i];
            }
        }

        printf("[kalibrasi]iter=%d exposure=%d us maxVal=%d\n",
               iter + 1, spektroExposureUs, maxVal);

        if (maxVal >= SPEKTRO_AE_MIN && maxVal <= SPEKTRO_AE_MAX) {
            converged = 1;
            break;
        }

        if (maxVal == 0) {
            maxVal = 1;
        }

        float target = ((float)SPEKTRO_AE_MIN + (float)SPEKTRO_AE_MAX) / 2.0f;
        long nextExposure = (long)((float)spektroExposureUs * (target / (float)maxVal));
        spektroExposureUs = clampExposure(nextExposure);
    }

    return converged;
}

int spektroCaptureReference(void)
{
    if (!captureCamera()) {
        return 0;
    }

    for (int i = 0; i < SPEKTRO_PIXEL_COUNT; i++) {
        spektroReference[i] = spektroValue[i];
    }
    spektroHasReference = 1;
    return 1;
}

int spektroScanAbsorbance(float *outData, int outCount)
{
    if (!spektroHasReference) {
        clearAbsorbance();
        fillInterpolatedOutput(outData, outCount);
        return 0;
    }

    if (!captureCamera()) {
        clearAbsorbance();
        fillInterpolatedOutput(outData, outCount);
        return 0;
    }

    for (int i = 0; i < SPEKTRO_PIXEL_COUNT; i++) {
        float I = (float)spektroValue[i];
        float I0 = (float)spektroReference[i];
        if (I > 0.0f && I0 > 0.0f) {
            spektroAbsorbance[i] = log10f(I0 / I);
        } else {
            spektroAbsorbance[i] = 0.0f;
        }
    }

    fillInterpolatedOutput(outData, outCount);
    return 1;
}

float spektroPeakAbsorbance(void)
{
    float peak = spektroAbsorbance[0];
    for (int i = 1; i < SPEKTRO_PIXEL_COUNT; i++) {
        if (spektroAbsorbance[i] > peak) {
            peak = spektroAbsorbance[i];
        }
    }
    return peak;
}

}
