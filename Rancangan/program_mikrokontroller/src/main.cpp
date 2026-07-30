#include <Arduino.h>
#include <Wire.h>
#include <esp_system.h>
#include "SharedData.h"
#include "Config.h"
#include "PompaShared.h"
#include "SpektroShared.h"

// Deklarasi Task Headers (Akan diimplementasikan di masing-masing .h/.cpp)
// Task Prototypes dipindah ke SharedData.h

// Definisi Real Global Variables (dideklarasikan extern di SharedData.h)
QueueHandle_t queueSensorPH; 
QueueHandle_t queueSensorSuhu;
QueueHandle_t queueSensorGas;
QueueHandle_t queueSensorArus;
QueueHandle_t queueSensorRPM;

QueueHandle_t queueSuhuForHeater;
QueueHandle_t queueSuhuForStirrer;

QueueHandle_t queueScheduleParam;
QueueHandle_t queueForceStop;

SemaphoreHandle_t mutexSystemState;
SemaphoreHandle_t mutexI2C;
//SemaphoreHandle_t mutexHallData;
SystemState currentSystemState = STATE_INISIALISASI_INPUT;
ScheduleParam currentActiveParam = {"", 0.0, 0, 0};
int currentRemainingTime = 0;
volatile int hallTimeoutFlag = 0;
Adafruit_ADS1115 ads;

extern "C" {
    /* Pompa */
    QueueHandle_t queueInisialisasiCmd;
    QueueHandle_t queueFlushingCmd;
    InisialisasiProgress currentInitProgress = {0, 0, 0, 0, -1};
    FlushingProgress     currentFlushProgress = {0};
    volatile int         initDoneFlag  = 0;
    volatile int         flushDoneFlag = 0;

    /* Spektrofotometer */
    QueueHandle_t queueSpektroCmd;
    DegradasiPoint lastDegradaPoint = {0, 0, 0.0f, 0};
    SpektrumResult lastSpektrum;   /* .data[] nol-init dari BSS, .ready = 0 */
    DegradasiSyncState degradaSync = {0, 0, 0, 0, 0, 0};
    SpektrumSyncState  spektrumSync = {0, 0, 0, 0, 0};
}

static const char *resetReasonStr(esp_reset_reason_t r) {
    switch (r) {
        case ESP_RST_POWERON:   return "POWERON (cold boot / supply mati total)";
        case ESP_RST_BROWNOUT:  return "BROWNOUT (tegangan drop -> SUSPECT HALOGEN INRUSH)";
        case ESP_RST_PANIC:     return "PANIC (firmware crash / Guru Meditation)";
        case ESP_RST_INT_WDT:   return "INT_WDT (interrupt watchdog)";
        case ESP_RST_TASK_WDT:  return "TASK_WDT (task watchdog)";
        case ESP_RST_WDT:       return "WDT (other watchdog)";
        case ESP_RST_SW:        return "SW (esp_restart / software)";
        case ESP_RST_DEEPSLEEP: return "DEEPSLEEP";
        case ESP_RST_EXT:       return "EXT (pin reset eksternal)";
        case ESP_RST_SDIO:      return "SDIO";
        default:                return "UNKNOWN";
    }
}

void setup() {
    // [FIX] Naikkan RX buffer sebelum Serial.begin() agar JSON jadwal panjang
    // (MAX_SCHEDULES=20 → ~500 bytes) tidak overflow buffer hardware 256 bytes default.
    Serial.setRxBufferSize(1024);
    Serial.begin(115200);
    delay(300); /* tunggu serial siap agar baris reset-reason kebaca */
    esp_reset_reason_t _rr = esp_reset_reason();
    Serial.printf("\n[RESET-REASON] code=%d -> %s\n", (int)_rr, resetReasonStr(_rr));
    // Inisialisasi I2C Bus untuk ADS1115 dll
    Wire.begin(GPIO_I2C_SDA, GPIO_I2C_SCL); 

    // 1. Buat Mutex
    mutexSystemState = xSemaphoreCreateMutex();
    mutexI2C = xSemaphoreCreateMutex();
    //mutexHallData = xSemaphoreCreateMutex();

    // 2. Buat Queues
    queueSensorPH = xQueueCreate(QUEUE_SENSOR_LATEST_LEN, sizeof(float));
    queueSensorSuhu = xQueueCreate(QUEUE_SENSOR_LATEST_LEN, sizeof(float));
    queueSensorGas = xQueueCreate(QUEUE_SENSOR_LATEST_LEN, sizeof(GasStatus));
    queueSensorArus = xQueueCreate(QUEUE_SENSOR_LATEST_LEN, sizeof(float));
    queueSensorRPM = xQueueCreate(QUEUE_SENSOR_LATEST_LEN, sizeof(float));

    queueSuhuForHeater = xQueueCreate(QUEUE_SENSOR_CTRL_LEN, sizeof(float));
    queueSuhuForStirrer = xQueueCreate(QUEUE_SENSOR_CTRL_LEN, sizeof(float));

    queueScheduleParam = xQueueCreate(QUEUE_PARAM_LEN, sizeof(ScheduleArray));
    queueForceStop = xQueueCreate(QUEUE_CMD_LEN, sizeof(int));

    queueInisialisasiCmd = xQueueCreate(1, sizeof(InisialisasiConfig));
    queueFlushingCmd     = xQueueCreate(1, sizeof(int));
    queueSpektroCmd      = xQueueCreate(4, sizeof(SpektroCmd));

    bool mutexCreationFailed = (mutexSystemState == NULL );
    bool queueCreationFailed = (queueSensorPH == NULL || queueScheduleParam == NULL
                                || queueInisialisasiCmd == NULL || queueFlushingCmd == NULL
                                || queueSpektroCmd == NULL);
    
    if(mutexCreationFailed || queueCreationFailed) {
        Serial.println("Gagal membuat Mutex / Queue! Sistem berhenti.");
        while(1);
    }

    // 3. Register Tasks
    // Parameter: (TaskFunction, Name, StackDepth, Parameters, Priority, TaskHandle)
    // --- Sensor Tasks ---
    xTaskCreate(taskBacaPH, "Task_PH", 2048, NULL, PRIORITY_SENSOR, NULL);
    xTaskCreate(taskBacaSuhu, "Task_Suhu", 2048, NULL, PRIORITY_SENSOR, NULL);
    xTaskCreate(taskBacaGas, "Task_Gas", 2048, NULL, PRIORITY_SENSOR, NULL);
    xTaskCreate(taskBacaSensorArus, "Task_Arus", 2048, NULL, PRIORITY_SENSOR, NULL);
    xTaskCreate(taskBacaSensorHall, "Task_Hall", 2048, NULL, PRIORITY_SENSOR, NULL);

    // --- Control Tasks ---
    xTaskCreate(taskKontrolStirrer, "Task_Stirrer", 2048, NULL, PRIORITY_KONTROL, NULL);
    xTaskCreate(taskKontrolHeater, "Task_Heater", 2048, NULL, PRIORITY_KONTROL, NULL);

    // --- Logic & Communication ---
    // [FIX] Stack dinaikkan ke 4096: ScheduleArray(MAX_SCHEDULES=20) ~900 bytes di stack lokal
    xTaskCreate(taskTimer, "Task_Timer", 4096, NULL, PRIORITY_TIMER, NULL);
    xTaskCreate(taskKomunikasi, "Task_Komunikasi", 8192, NULL, PRIORITY_KOMUNIKASI, NULL);

    // --- Pompa (Inisialisasi & Flushing) ---
    xTaskCreate(taskManajemenPompa, "Task_Pompa", 4096, NULL, PRIORITY_POMPA, NULL);

    // --- Spektrofotometer (stack lebih besar karena scan data float array) ---
    xTaskCreate(taskBacaSpektrum, "Task_Spektro", 4096, NULL, PRIORITY_SENSOR, NULL);

    Serial.println("===== Sistem RTOS Reaktor Siap =====");
    Serial.println("Menunggu Perintah (Kirim JSON / UI Command)...");
}

void loop() {
    // Di ESP32 RTOS, loop() adalah task priority 1. Bisa kita hapus.
    vTaskDelete(NULL); 
}