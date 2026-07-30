#ifndef CONFIG_H
#define CONFIG_H

// --- Hardware Pins ---
// Berdasarkan skema footprint ESP32
#define GPIO_I2C_SDA     21 // Pin 26 di modul -> GPIO21
#define GPIO_I2C_SCL     22 // Pin 29 di modul -> GPIO22
 
#define PIN_ONEWIRE_SUHU 4  // Pin 20 di modul -> GPIO4 (suhu_Out)
#define GPIO_PWM_STIRRER 27 // Pin 6  di modul -> GPIO27 (PWM_Stirrer)
#define GPIO_HALL_STIRRER 33 // Pin Hall sensor stirrer -> GPIO33
#define GPIO_PWM_HEATER  16 // Pin 21 di modul -> GPIO16 (PWM_SSR)
#define GPIO_PWM_H2SO4 32 // Pin 10 di modul -> GPIO32 (PWM_H2SO4)
#define GPIO_PWM_FESO4 19 // Pin 25 di modul -> GPIO19 (PWM_FeSO4)
#define GPIO_PWM_SAMPLE 18 // Pin 23 di modul -> GPIO5 (PWM_Sample)
#define GPIO_PWM_WASTE_CONTAINER 5 // Pin 24 di modul -> GPIO18 (PWM_WasteContainer)
#define GPIO_PWM_AIR 12 // Pin 4 di modul -> GPIO12 (PWM_Air)
#define GPIO_PWM_KUVET 23 // Pin 30 di modul -> GPIO23 (PWM_Kuvet)
#define GPIO_PWM_AERATOR 13 // Pin PWM baru untuk aerator
#define GPIO_PWM_HALOGEN 17 // Pin PWM lampu halogen spektrofotometer
#define GPIO_SPEKTRO_CLOCK 2 // Clock sensor spektrofotometer
#define GPIO_SPEKTRO_SI 26 // SI sensor spektrofotometer
#define GPIO_PWM_FAN   14   // Fan aktif saat proses EF berjalan
#define GPIO_DAC_ARUS  25   // DAC output kontrol sumber arus

// (Sensor Gas menggunakan ADS1115 pada input A2, tidak menggunakan pin analog ESP32)

// --- RTOS Config ---
// Ukuran Queue
#define QUEUE_SENSOR_LATEST_LEN 1
#define QUEUE_SENSOR_CTRL_LEN 5
#define QUEUE_PARAM_LEN  3
#define QUEUE_CMD_LEN    2

// Prioritas Task (Makin tinggi makin prioritas)
#define PRIORITY_SENSOR   2
#define PRIORITY_KONTROL  3
#define PRIORITY_TIMER    4
#define PRIORITY_KOMUNIKASI 1 // Komunikasi bisa rendah, jalan saat tidak sibuk kontrol

// Delay / Periode Task (ms) - portTICK_PERIOD_MS digunakan di C++ RTOS
#define PERIOD_BACA_PH 1000
#define PERIOD_BACA_SUHU 250 // Sesuai contoh (delay 250)
#define PERIOD_BACA_GAS 4000
#define PERIOD_KONTROL 500
#define PERIOD_STIRRER_CONTROL_MS 20
#define PERIOD_STREAM 1000 // UI update tiap 1 detik

// --- Kalibrasi Sensor ---
// Koreksi drift sensor pH: nilai terbaca terlalu rendah sebesar 1 unit (pH 3 terbaca jadi pH 2)
#define PH_DRIFT_OFFSET        1.0f
// Threshold delta tegangan (mV) antar sample untuk deteksi CO2 pada sensor gas
// Ditiup (CO2) -> tegangan turun >= 1mV; Tidak ditiup -> tegangan naik >= 1mV
#define GAS_DELTA_THRESHOLD_MV  1.0f
// Rentang tegangan "normal" (mV) saat tidak ada CO2; delta turun di luar rentang ini diabaikan
#define GAS_RANGE_LOW_MV        20.0f  // Batas bawah rentang valid (inklusif)
#define GAS_RANGE_HIGH_MV       25.0f  // Batas atas rentang valid (inklusif)
// Batas absolut bawah (mV): tegangan di bawah ini langsung dianggap CO2 detected
#define GAS_ABS_LOW_MV          19.0f

// --- Pompa Task Config ---
#define PRIORITY_POMPA         3     // Sama dengan PRIORITY_KONTROL
#define PERIOD_POMPA_SIMULASI  500   // ms per step simulasi dummy pompa

// --- Kuvet Prefilling ---
// Fluida membutuhkan waktu untuk mengalir dari reaktor ke kuvet sebelum siap diukur.
// Ubah nilai-nilai ini jika panjang selang atau laju alir pompa kuvet berubah.
// Satuan: milidetik (ms). Contoh: 20 detik = 20000 ms, 30 detik = 30000 ms
#define KUVET_PREFILL_FIRST_DEGRADA_MS  10000  // Prefill sebelum pengukuran DEGRADASI PERTAMA (t=0)
#define KUVET_PREFILL_SPEKTRUM_MS       10000  // Prefill sebelum setiap scan SPEKTRUM PENUH

// --- Sumber Arus (EF) ---
// Lonjakan arus langsung ke nilai target dapat membuat alat mati (overcurrent trip).
// Arus dinaikkan bertahap secara linear dari 0 mA sampai target (input user) selama
// ARUS_RAMP_MS, baru dipertahankan di nilai target untuk sisa durasi reaksi EF.
#define ARUS_RAMP_MS 30000UL

#endif
