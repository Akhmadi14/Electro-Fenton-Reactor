/*
 * taskKalibrasiSpektro.c
 *
 * Modul kalibrasi spektrofotometer.
 * Dipanggil oleh taskManajemenPompa saat fase kalibrasi inisialisasi.
 *
 * Protokol: setelah selesai, kalibrasiStatus di currentInitProgress
 * diset menjadi 1, yang kemudian di-stream oleh taskKomunikasi sebagai
 * {"status":"ongoing/done","kalibrasi":1,...}
 *
 * Mengontrol hardware spektrofotometer untuk menyimpan referensi blanko.
 */

#include "PompaShared.h"
#include "SpektroShared.h"
#include <stdio.h>

/* mutexSystemState dipakai bersama oleh semua modul */
extern SemaphoreHandle_t mutexSystemState;

/*
 * PANDUAN INTEGRASI HARDWARE:
 * Saat kalibrasi, simpan nilai I0 (intensitas referensi tanpa sampel)
 * untuk tiap panjang gelombang yang akan digunakan. Nilai I0 ini
 * kemudian dipakai oleh taskBacaSpektrum saat menghitung absorbansi:
 *     A = -log10(I / I0)
 * Simpan array I0 sebagai variabel global di SpektroShared.h bila perlu.
 */

void runKalibrasiSpektro(void)
{
    printf("[kalibrasi] mulai (halogen ON)\n");
    spektroHardwareInit();
    spektroSetHalogenDuty(SPEKTRO_HALOGEN_DUTY_ON);
    vTaskDelay(pdMS_TO_TICKS(SPEKTRO_PREPARE_DELAY_MS));

    int konvergen = spektroAutoExposure();
    printf("[kalibrasi]exposure=%d us konvergen=%d\n", spektroExposureUs, konvergen);
    vTaskDelay(pdMS_TO_TICKS(SPEKTRO_CALIB_REF_DELAY_MS));

    int refOk = spektroCaptureReference();
    int refMax = 0, refMin = 65535;
    for (int i = 0; i < SPEKTRO_PIXEL_COUNT; i++) {
        if (spektroReference[i] > refMax) refMax = spektroReference[i];
        if (spektroReference[i] < refMin) refMin = spektroReference[i];
    }
    printf("[kalibrasi]referensi ok=%d hasRef=%d refMin=%d refMax=%d\n",
           refOk, spektroHasReference, refMin, refMax);

    vTaskDelay(pdMS_TO_TICKS(SPEKTRO_SETTLE_DELAY_MS));
    spektroSetHalogenDuty(0);
    printf("[kalibrasi]selesai (halogen OFF)\n");

    if (xSemaphoreTake(mutexSystemState, portMAX_DELAY) == pdTRUE) {
        currentInitProgress.kalibrasiStatus = 1;
        xSemaphoreGive(mutexSystemState);
    }
}
