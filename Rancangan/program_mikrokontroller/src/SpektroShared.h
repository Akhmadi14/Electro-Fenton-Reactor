#ifndef SPEKTRO_SHARED_H
#define SPEKTRO_SHARED_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPEKTRO_PIXEL_START 0
#define SPEKTRO_PIXEL_END 127
#define SPEKTRO_PIXEL_COUNT (SPEKTRO_PIXEL_END - SPEKTRO_PIXEL_START + 1)

/* Kirim per-pixel mentah (tanpa interpolasi): 128 titik = jumlah pixel terbaca */
#define SPEKTRUM_POINTS SPEKTRO_PIXEL_COUNT
#define SPEKTRO_DEFAULT_EXPOSURE_US 1000
#define SPEKTRO_AE_MIN 16000
#define SPEKTRO_AE_MAX 26000
#define SPEKTRO_AE_EXP_MIN 100
#define SPEKTRO_AE_EXP_MAX 100000
#define SPEKTRO_AE_MAX_ITER 20
#define SPEKTRO_HALOGEN_DUTY_ON 255

/* Delay agar alur baca spektro realistis dan UI punya waktu memproses */
#define SPEKTRO_PREPARE_DELAY_MS 1000
#define SPEKTRO_SETTLE_DELAY_MS  500
#define SPEKTRO_CALIB_REF_DELAY_MS 500
#define SPEKTRO_SERIAL_GUARD_MS  1000
#define SPEKTRUM_SERIAL_GUARD_MS 2000

/* Jenis perintah ke taskBacaSpektrum */
#define SPEKTRO_CMD_DEGRADASI 1  /* ukur puncak absorbansi (untuk degradasi) */
#define SPEKTRO_CMD_SPEKTRUM  2  /* scan penuh 400-800nm (untuk spektrum warna) */

/* Perintah yang dikirim ke queue taskBacaSpektrum */
typedef struct {
    int cmdType;    /* SPEKTRO_CMD_DEGRADASI atau SPEKTRO_CMD_SPEKTRUM */
    int reaksiId;   /* id reaksi asal (dari ScheduleParam.id) */
    int t_menit;    /* waktu dalam menit, relevan hanya untuk DEGRADASI */
} SpektroCmd;

/* Hasil pengukuran puncak absorbansi (untuk grafik degradasi) */
typedef struct {
    int   reaksiId;
    int   t_menit;
    float absorbansi;
    int   ready;  /* 1 = data valid, dikonsumsi oleh taskKomunikasi */
} DegradasiPoint;

/* Hasil scan spektrum penuh 400-800nm */
typedef struct {
    int   id;
    float data[SPEKTRUM_POINTS];  /* 128 pixel: indeks 0 = 400nm, indeks 127 = 800nm */
    int   ready;  /* 1 = data valid, dikonsumsi oleh taskKomunikasi */
} SpektrumResult;

/* Queue perintah ke taskBacaSpektrum (panjang 4, jangan overflow) */
extern QueueHandle_t queueSpektroCmd;

/* Shared results — dilindungi mutexSystemState saat akses flag ready */
extern DegradasiPoint lastDegradaPoint;
extern SpektrumResult lastSpektrum;

/* Sinkronisasi antar taskTimer, taskBacaSpektrum, dan taskKomunikasi */
typedef struct {
    int busy;          /* 1 = command sudah dikirim dan masih diproses */
    int produced;      /* 1 = hasil ukur sudah siap di shared result */
    int sent;          /* 1 = hasil sudah benar-benar dikirim ke Serial */
    int reaksiId;
    int t_menit;
    uint32_t sentAtMs; /* millis() saat JSON keluar ke Serial */
} DegradasiSyncState;

typedef struct {
    int busy;          /* 1 = scan spektrum sedang berjalan / menunggu dikirim */
    int produced;      /* 1 = array spektrum selesai diisi */
    int sent;          /* 1 = payload panjang sudah selesai dikirim ke Serial */
    int reaksiId;
    uint32_t sentAtMs; /* millis() saat JSON keluar ke Serial */
} SpektrumSyncState;

extern DegradasiSyncState degradaSync;
extern SpektrumSyncState  spektrumSync;

extern uint16_t spektroValue[SPEKTRO_PIXEL_COUNT];
extern uint16_t spektroReference[SPEKTRO_PIXEL_COUNT];
extern float    spektroAbsorbance[SPEKTRO_PIXEL_COUNT];
extern int      spektroHasReference;
extern int      spektroExposureUs;
extern uint32_t spektroLastExposureUs;

void spektroHardwareInit(void);
void spektroSetHalogenDuty(uint32_t duty);
int  spektroAutoExposure(void);
int  spektroCaptureReference(void);
int  spektroScanAbsorbance(float *outData, int outCount);
float spektroPeakAbsorbance(void);

/* FreeRTOS task — didefinisikan di taskBacaSpektrum.c */
void taskBacaSpektrum(void *pvParameters);

#ifdef __cplusplus
}
#endif

#endif /* SPEKTRO_SHARED_H */
