// Header dihapus, prototipe dipindah ke SharedData.h
#include <OneWire.h>
#include <DallasTemperature.h>
#include "SharedData.h"
#include "Config.h"

// Setup instance OneWire dan DallasTemperature
OneWire oneWireSuhu(PIN_ONEWIRE_SUHU);
DallasTemperature sensorsSuhu(&oneWireSuhu);

void taskBacaSuhu(void *pvParameters) {
    // ==== SETUP AWAL TASK ====
    sensorsSuhu.begin();
    Serial.println("[Init] Sistem Pemantauan Suhu Siap!");

    // ==== LOOPING TASK ====
    for(;;) {
        // Meminta suhu
        sensorsSuhu.requestTemperatures(); 

        // Membaca suhu dalam Celcius dari Sensor Pertama (Index 0)
        float temperatureC = sensorsSuhu.getTempCByIndex(0);

        if (temperatureC == DEVICE_DISCONNECTED_C) {
            //Serial.println("[Error] Sensor Suhu (DS18B20) tidak terdeteksi!");
            temperatureC = 10.0; // Nilai error khas DS18B20
        }
         // Update queue khusus bacaan suhu (untuk stream JSON/UI)
        xQueueOverwrite(queueSensorSuhu, &temperatureC);

            // Supaya tidak menghalangi aliran, kita gunakan xQueueSend tanpa block (delay 0)
            // ke control task (stirrer dan heater) yg mungkin membutuhkannya untuk referensi
        xQueueSend(queueSuhuForHeater, &temperatureC, 0);
        xQueueSend(queueSuhuForStirrer, &temperatureC, 0);
        

        // Delay 250ms (sesuai instruksi user)
        vTaskDelay(pdMS_TO_TICKS(PERIOD_BACA_SUHU));
    }
}
