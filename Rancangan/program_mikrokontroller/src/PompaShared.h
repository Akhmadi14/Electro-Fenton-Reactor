#ifndef POMPA_SHARED_H
#define POMPA_SHARED_H

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Konfigurasi inisialisasi yang dikirim UI via UART JSON */
typedef struct {
    int   vol;       /* volume sampel mL (50-250) */
    float ph;        /* target pH, -1 = skip */
    float fe;        /* jumlah Fe2+ mm, -1 = skip */
    char  fe_type[4];/* "ml" atau "mm" */
    int   kalibrasi; /* 0 = tidak, 1 = kalibrasi spektro */
    int   prefilling; /* 1 = lakukan prefill, -1 = tidak */
} InisialisasiConfig;

/* Progress inisialisasi — di-stream ke UI oleh taskKomunikasi */
typedef struct {
    int sampelPercent;   /* 0-100 */
    int h2so4Status;     /* -1=skip, 0=proses, 1=selesai */
    int feso4Status;     /* -1=skip, 0=proses, 1=selesai */
    int kalibrasiStatus; /* -1=skip, 0=proses, 1=selesai */
    int prefillStatus;   /* -1=tidak prefill, 0=sedang, 1=selesai */
} InisialisasiProgress;

/* Progress flushing — di-stream ke UI oleh taskKomunikasi */
typedef struct {
    int progressPercent; /* 0-100 */
} FlushingProgress;

/* Queue untuk mengirim perintah ke task pompa */
extern QueueHandle_t queueInisialisasiCmd;
extern QueueHandle_t queueFlushingCmd;

/* Shared progress (dilindungi mutexSystemState) */
extern InisialisasiProgress currentInitProgress;
extern FlushingProgress     currentFlushProgress;
extern volatile int         initDoneFlag;   /* 0 = belum, 1 = selesai */
extern volatile int         flushDoneFlag;  /* 0 = belum, 1 = selesai */

/* FreeRTOS task — didefinisikan di taskManajemenPompa.c (C file) */
void taskManajemenPompa(void *pvParameters);

/* Fungsi kalibrasi spektrofotometer — didefinisikan di taskKalibrasiSpektro.c */
void runKalibrasiSpektro(void);

#ifdef __cplusplus
}
#endif

#endif /* POMPA_SHARED_H */
