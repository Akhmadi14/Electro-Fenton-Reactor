# Reaktor RTOS Project (Phase 1)

Proyek ini adalah implementasi sistem kontrol Reaktor menggunakan arsitektur **FreeRTOS** pada ESP32. Sistem ini dirancang untuk multitasking yang efisien dengan pembagian task sensor, kontrol PWM, state-machine (timer), dan komunikasi antar muka (UI).

---

## 🔌 Pemetaan Perangkat Keras (Hardware Pins)

| Modul | Referensi Footprint / Alat | Pin ESP32 (GPIO) | Keterangan |
| :--- | :--- | :--- | :--- |
| **SDA (I2C)** | `SDA` (Pin 26 di modul) | `GPIO21` | Bus I2C Utama |
| **SCL (I2C)** | `SCL` (Pin 29 di modul) | `GPIO22` | Bus I2C Utama |
| **Sensor Suhu (DS18B20)** | `suhu_Out` (Pin 20) | `GPIO4` | OneWire Data |
| **PWM Stirrer** | `PWM_Stirrer` (Pin 6) | `GPIO27` | Kontrol Kecepatan Stirrer (PWM Channel 0) |
| **PWM Heater (SSR)** | `PWM_SSR` (Pin 21) | `GPIO16` | Kontrol Pemanasan (PWM Channel 1) |
| **Sensor pH** | `pH_Out` di ADS1115 | `A1` (ADS1115) | Via I2C (Address 0x48) |
| **Sensor Gas (MQ-135)** | `gas_Out` di ADS1115 | `A2` (ADS1115) | Via I2C (Address 0x48) |

---

## 💻 Panduan Komunikasi Serial (User Interface)

Sistem sudah dirancang agar dapat merespons dan mengirimkan pembaruan kepada antarmuka pengguna (UI) melalui komunikasi Serial di **Baudrate 115200**. Karena Anda sudah mengaktifkan `send_on_enter` dan `monitor_echo` di PlatformIO, Anda dapat langsung mengetik instruksi pada terminal PlatformIO.

Terdapat kondisi state:
*   **IDLE** : Reaktor siaga, siap menerima jadwal reaksi.
*   **EKSEKUSI** : Reaktor sedang menjalankan rutinitas sesuai durasi jadwal.

### 1. Data Stream (ESP32 ➔ UI)
Setiap periodik, ESP32 akan mengirimkan status reaktor dalam bentuk `JSON String`.  

**Format Stream saat IDLE:**
```json
{"State":"Idle","pH":7.23,"suhu":28.50,"gas":12.45}
```

**Format Stream saat EKSEKUSI:**
*(Catatan: Parameter `pH` dan `gas` hanya akan tampil jika reaksi yang sedang berjalan membutuhkannya, misalnya untuk reaksi "ef".)*
```json
{
  "reaksi": "ef",
  "parameter": {
    "suhu": 37.5,
    "pH": 5.2,
    "waktu_tersisa": 120,
    "gas": 120
  },
  "State": "Eksekusi"
}
```

### 2. Memulai Jadwal Reaksi (UI ➔ ESP32)
Untuk menyalakan reaksi dan memindahkan sistem dari `IDLE` ke `EKSEKUSI` selama beberapa interval waktu.

**Format Perintah Command UI (Kirim String Serial):**
Kirimkan *JSON Object* yang berisi status start beserta array urutan jadwal(reaksi) ke terminal Serial.

**Contoh Payload Start (Kopi-Paste langsung ke terminal/monitor serial, satu baris):**
```json
{"status":"start","jadwal":[{"reaksi":"ef","suhu":25,"rpm":1000,"durasi":7200},{"reaksi":"adsorpsi","suhu":25,"rpm":1000,"durasi":1200}]}
```

**Balasan (Acknowledge) jika berhasil:**
```json
{"ACK":"Start Jadwal Diterima"}
```

### 3. Menghentikan Paksa Jadwal (FORCE STOP) (UI ➔ ESP32)
Jika sedang di tengah reaksi (`EKSEKUSI`) atau ada kondisi darurat dan Anda ingin mematikan semua kegiatan untuk kembali ke `IDLE`.

**Format Perintah Command:**
```text
FORCE_STOP
```

**Contoh Penggunaan:**
👉 *Ketik di Terminal:* `FORCE_STOP` lalu tekan `Enter`.

**Acknowledge dan Pemberitahuan Eksekusi Selesai:**
Jika di-stop maka akan muncul `ACK:FORCE_STOP`.
Jika sekumpulan antrean jadwal selesai seluruhnya secara alami tanpa paksaan, ESP32 akan mencetak `EXECUTE_DONE`.

---

## 🛠️ Catatan Tambahan FreeRTOS
Sistem dilindungi menggunakan sistem _Queue_ dan _Semaphore/Mutex_:
1.  **I2C Mutex**: Memastikan bahwa modul ADS1115 (pembaca gas dan pH) tidak _overlap_ I2C request-nya karena diakses oleh 2 task bersamaan.
2.  **Queue**: Setiap sensor membaca ke Queue. Task Komunikasi mengawasi Queue dan mengubah datanya menjadi JSON stream. Task Kontrol Suhu menyadap antrian_Suhu_ untuk menjalankan sistem Pemanas secara _pseudo-PID / Bang-Bang_.

---
*Tetap utamakan Keselamatan saat merangkai kelistrikan (terutama SSR Heater)!*
