// Header dihapus, prototipe dipindah ke SharedData.h
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "SharedData.h"
#include "Config.h"

extern Adafruit_ADS1115 ads;
extern SemaphoreHandle_t mutexI2C;
extern QueueHandle_t queueSensorPH;

void taskBacaPH(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    Serial.println("[Init] Sensor pH memulai...");
    
    // Inisialisasi ADS1115. Alamat I2C default adalah 0x48 (pin ADDR ke GND)
    // Gunakan mutex karena ADS terpakai di taskGas juga (saat initialization phase tidak apa apa karena tasGas juga baru boot)
    if(xSemaphoreTake(mutexI2C, portMAX_DELAY) == pdTRUE) {
        if (!ads.begin(0x48, &Wire)) {
            Serial.println("[Error] Gagal menemukan ADS1115!");
            xSemaphoreGive(mutexI2C);
            while (1) {
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
        // GAIN_TWOTHIRDS untuk rentang +/- 6.144V (cocok sensor 0-5V)
        ads.setGain(GAIN_TWOTHIRDS);
        xSemaphoreGive(mutexI2C);
    }
    Serial.println("[Ready] Sensor pH Siap!");

    // ==== LOOPING TASK ====
    for(;;) {
        int16_t adc1 = 0;
        float voltage = 0.0f;
        
        // Membaca dari pin A1 (index 1) - sesuai instruksi dan aman dengan mutex
        if(xSemaphoreTake(mutexI2C, portMAX_DELAY) == pdTRUE) {
            ads.setGain(GAIN_TWOTHIRDS);
            adc1 = ads.readADC_SingleEnded(1);
            voltage = ads.computeVolts(adc1);
            xSemaphoreGive(mutexI2C);
        }

        // Menghitung pH menggunakan rumus kalibrasi TERBARU
        float phValue = (-9.09f * voltage) + 21.7;
        // Koreksi drift sensor: terbaca terlalu rendah, tambahkan offset (lihat Config.h)
        phValue += PH_DRIFT_OFFSET;

        // Debug log khusus via Serial bisa opsional (aktifkan jika debugging tanpa UI)
        // Serial.printf("V_A1: %.3f V | pH: %.2f\n", voltage, phValue);

        // Mengirim ke queueSensorPH agar terbaca di task lain (misal Task Komunikasi)
        xQueueOverwrite(queueSensorPH, &phValue); // Gunakan xQueueOverwrite bila antrian max cuma butuh 1 value mutakhir

        // Memberikan jeda CPU agar task lain bisa berjalan
        vTaskDelay(pdMS_TO_TICKS(PERIOD_BACA_PH));
    }
}
