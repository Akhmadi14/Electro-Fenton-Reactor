#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <Adafruit_ADS1X15.h>

// Definisi State Reaktor
enum SystemState {
    STATE_IDLE,
    STATE_EKSEKUSI,
    STATE_INISIALISASI_INPUT,
    STATE_INISIALISASI_EKSEKUSI,
    STATE_FLUSHING
};

// Struktur Data Sensor untuk streaming
struct SensorData {
    float pH;
    float suhu;
    float gas;
    SystemState state;
    int currentReaction; // Reaksi aktif jika ada, 0 jika tidak ada
};

#define GAS_STATUS_LEN 20

struct GasStatus {
    char value[GAS_STATUS_LEN];
};

// Struktur Parameter Jadwal/Reaksi dari input JSON UI
// Naikkan batas agar urutan step campuran (ef/adsorpsi/spektro) tidak terpotong.
// [FIX] Dinaikkan ke 20 — UI bisa kirim >10 step "spektro" berurutan.
#define MAX_SCHEDULES 20

struct ScheduleParam {
    char reaksi[16];     // "ef", "adsorpsi", "spektro", dll
    float suhu;          // Target suhu
    int rpm;             // Target RPM stirrer
    int durasi;          // Durasi dalam detik
    int arus;            // Target Arus (mA)
    int airflow_level;   // Level Airflow
    bool ukur_degradasi; // Opsi ukur degradasi warna tiap 5 menit
    int id;              // Identifikasi reaksi (dikirim UI, diteruskan ke stream)
};

struct ScheduleArray {
    int count;
    ScheduleParam schedules[MAX_SCHEDULES];
};

// Konstanta Command dari UI String
#define CMD_START "START_SCHEDULE"
#define CMD_STOP "FORCE_STOP"

// Deklarasi Global Queues dan Mutex
// Digunakan `extern` agar bisa diakses di semua task yang meng-include SharedData.h
extern QueueHandle_t queueSensorPH; 
extern QueueHandle_t queueSensorSuhu;
extern QueueHandle_t queueSensorGas;
extern QueueHandle_t queueSensorArus; // Queue pembacaan Arus Dummy
extern QueueHandle_t queueSensorRPM;  // Queue pembacaan RPM (Hall Sensor) Dummy

extern QueueHandle_t queueSuhuForHeater;
extern QueueHandle_t queueSuhuForStirrer;

// Queue untuk mengirim command perubahan state dari Komunikasi ke Timer
extern QueueHandle_t queueScheduleParam;
extern QueueHandle_t queueForceStop;

// Mutex untuk melindungi akses ke state global dan I2C
extern SemaphoreHandle_t mutexSystemState;
extern SemaphoreHandle_t mutexI2C;
extern SemaphoreHandle_t mutexHallData;

extern SystemState currentSystemState;
extern ScheduleParam currentActiveParam; // Parameter reaksi yang sedang berjalan
extern int currentRemainingTime;         // Sisa waktu reaksi dalam detik
extern volatile int hallTimeoutFlag;
extern Adafruit_ADS1115 ads; // Objek ADS1115 yang bisa diakses task pH dan Gas

// Prototipe FreeRTOS Task
void taskBacaPH(void *pvParameters);
void taskBacaSuhu(void *pvParameters);
void taskBacaGas(void *pvParameters);
void taskKontrolStirrer(void *pvParameters);
void taskKontrolHeater(void *pvParameters);
void taskTimer(void *pvParameters);
void taskKomunikasi(void *pvParameters);
void taskBacaSensorArus(void *pvParameters);
void taskBacaSensorHall(void *pvParameters);

#endif
