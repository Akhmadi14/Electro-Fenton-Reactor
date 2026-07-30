#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;

#define clockPin  2
#define siPin     26
#define SDA_PIN   21
#define SCL_PIN   22

// Pixel Range (0-indexed, 122 pixel aktif)
#define PIXEL_START   0
#define PIXEL_END     127
#define PIXEL_COUNT   (PIXEL_END - PIXEL_START + 1)  // = 122

// Exposure awal
int exposure = 1000;  // µs, 

// === Auto Exposure Target ===
#define AE_MIN      16000    // ~50% dari 32767
#define AE_MAX      26000    // ~80% dari 32767
#define AE_EXP_MIN  10    // µs minimum
#define AE_EXP_MAX  100000   // µs maksimum
#define AE_MAX_ITER 20       // batas iterasi

// === Buffer ===
// ESP32 punya 520KB SRAM, tidak perlu khawatir ukuran array
uint16_t  Value[PIXEL_COUNT];       // pixel aktif (index 3-124)
uint16_t  Reference[PIXEL_COUNT];   // referensi blanko
float     Absorbance[PIXEL_COUNT];  // hasil absorbansi

bool hasReference = false;
bool hasScan      = false;
unsigned long utime_Exp = 0;

// === Wavelength mapping linear ===
// pixel 3   -> 400 nm
// pixel 124 -> 800 nm
float pixelToWavelength(int pixelIndex) {
  return 400.0 + (800.0 - 400.0) *
         ((float)(pixelIndex - PIXEL_START) / (float)(PIXEL_END - PIXEL_START));
}

// =====================================================
void setup() {
  pinMode(siPin,    OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(siPin,    LOW);

  Serial.begin(115200);  // ESP32 stabil di 115200
  delay(500);

  // Inisialisasi I2C dengan pin custom ESP32
  Wire.begin(SDA_PIN, SCL_PIN);

  // ADS1115 default address 0x48
  if (!ads.begin(0x48, &Wire)) {
    Serial.println("ADS1115 tidak ditemukan! Cek koneksi I2C.");
    while (1) { delay(100); }
  }
  ads.setGain(GAIN_TWO);        // ±4.096V, resolusi ~0.125 mV/bit
  ads.setDataRate(RATE_ADS1115_860SPS);  // sampling rate maksimum

  Serial.println("Pixel aktif : 3 - 124 (0-indexed), total 122 pixel");
  Serial.println("Rentang     : 400 nm - 800 nm");
  Serial.println("Perintah:");
  Serial.println("  auto             -> auto exposure otomatis (target 50-80%)");
  Serial.println("  ref              -> ambil referensi");
  Serial.println("  scan             -> ambil sampel & hitung absorbansi");
  Serial.println("  show absorbance  -> tampilkan absorbansi terakhir");
  Serial.println("  show intensity   -> tampilkan intensitas terakhir");
  Serial.println("  exposure=XXXX    -> set exposure manual dalam µs");
  Serial.println("  status           -> tampilkan status & setting saat ini");
  Serial.println("====================================================");
}

// =====================================================
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toLowerCase();

    if (cmd.startsWith("exposure=")) {
      int val = cmd.substring(9).toInt();
      if (val >= AE_EXP_MIN && val <= AE_EXP_MAX) {
        exposure = val;
        Serial.print("Exposure diset ke: ");
        Serial.print(exposure);
        Serial.println(" µs");
      } else {
        Serial.print("Nilai tidak valid. Rentang: ");
        Serial.print(AE_EXP_MIN);
        Serial.print(" - ");
        Serial.print(AE_EXP_MAX);
        Serial.println(" µs");
      }
    }

    // Auto Exposure 
    else if (cmd == "auto") {
      autoExposure();
    }

    // Ambil referensi
    else if (cmd == "ref") {
      getCamera();
      for (int i = 0; i < PIXEL_COUNT; i++) {
        Reference[i] = Value[i];
      }
      hasReference = true;
      Serial.println("Referensi tersimpan.");
      Serial.println("wavelength_nm,intensity_ref");
      printIntensity(Reference);
    }

    // Ambil sampel & hitung absorbansi
    else if (cmd == "scan") {
      if (!hasReference) {
        Serial.println("Belum ada referensi! Gunakan 'ref' terlebih dahulu.");
        return;
      }
      getCamera();
      hasScan = true;

      for (int i = 0; i < PIXEL_COUNT; i++) {
        float I  = (float)Value[i];
        float I0 = (float)Reference[i];
        if (I > 0.0 && I0 > 0.0) {
          Absorbance[i] = log10(I0 / I);
        } else {
          Absorbance[i] = 0.0;
        }
      }

      Serial.print("Waktu ekspose : ");
      Serial.print(utime_Exp);
      Serial.println(" µs");
      Serial.println("wavelength_nm,absorbance");
      printAbsorbance();
    }

    // Tampilkan absorbansi terakhir
    else if (cmd == "show absorbance") {
      if (!hasScan) {
        Serial.println("Belum ada data scan.");
        return;
      }
      Serial.println("wavelength_nm,absorbance");
      printAbsorbance();
    }

    // Tampilkan intensitas terakhir
    else if (cmd == "show intensity") {
      Serial.println("wavelength_nm,intensity");
      printIntensity(Value);
    }

    // Status sistem
    else if (cmd == "status") {
      Serial.println("--- Status Sistem ---");
      Serial.print("Exposure      : "); Serial.print(exposure); Serial.println(" µs");
      Serial.print("Referensi     : "); Serial.println(hasReference ? "Ada" : "Belum diambil");
      Serial.print("Data scan     : "); Serial.println(hasScan ? "Ada" : "Belum scan");
      Serial.print("Free heap     : "); Serial.print(ESP.getFreeHeap()); Serial.println(" bytes");
      Serial.println("---------------------");
    }

    else {
      Serial.print("Perintah tidak dikenal: ");
      Serial.println(cmd);
    }
  }
}

// =====================================================
void getCamera() {
  // --- SI pulse: mulai integrasi ---
  digitalWrite(clockPin, LOW);
  digitalWrite(siPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(siPin, LOW);
  digitalWrite(clockPin, LOW);

  // Clock out 128 siklus (periode integrasi)
  for (int j = 0; j < 128; j++) {
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }

  // Tunggu exposure
  utime_Exp = micros();
  delayMicroseconds(exposure);
  utime_Exp = micros() - utime_Exp;

  // --- SI pulse kedua: mulai readout ---
  digitalWrite(siPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(siPin, LOW);
  digitalWrite(clockPin, LOW);

  // --- Baca 128 pixel, simpan langsung hanya yang aktif ---
  for (int j = 0; j < 128; j++) {
    delayMicroseconds(20);  // settling time
    int16_t val = ads.readADC_SingleEnded(3);  // channel A0
    if (val < 0) val = 0;

    if (j >= PIXEL_START && j <= PIXEL_END) {
      Value[j - PIXEL_START] = (uint16_t)val;
    }

    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

// =====================================================
// Auto Exposure: sesuaikan exposure agar nilai puncak
// berada di rentang AE_MIN - AE_MAX (50-80% dari 32767)
// =====================================================
void autoExposure() {
  Serial.println("Memulai auto exposure...");

  int maxVal = 0;
  int iter   = 0;

  while (iter < AE_MAX_ITER) {
    getCamera();

    // Cari nilai maksimum dari pixel aktif
    maxVal = 0;
    for (int i = 0; i < PIXEL_COUNT; i++) {
      if (Value[i] > maxVal) maxVal = Value[i];
    }

    Serial.print("  Iter ");
    Serial.print(iter + 1);
    Serial.print(" | exposure=");
    Serial.print(exposure);
    Serial.print(" µs | maxVal=");
    Serial.println(maxVal);

    // Sudah di rentang target?
    if (maxVal >= AE_MIN && maxVal <= AE_MAX) break;

    // Hitung exposure baru secara proporsional
    float target = (AE_MIN + AE_MAX) / 2.0;  // target = 21000
    if (maxVal == 0) maxVal = 1;

    long newExposure = (long)((float)exposure * (target / (float)maxVal));
    exposure = (int)constrain(newExposure, AE_EXP_MIN, AE_EXP_MAX);

    iter++;
  }

  if (maxVal >= AE_MIN && maxVal <= AE_MAX) {
    Serial.print("Auto exposure selesai. Exposure = ");
    Serial.print(exposure);
    Serial.print(" µs | maxVal = ");
    Serial.println(maxVal);
  } else {
    Serial.println("PERINGATAN: Auto exposure tidak konvergen.");
    Serial.println("Periksa sumber cahaya atau atur manual.");
    Serial.print("Exposure terakhir = ");
    Serial.print(exposure);
    Serial.print(" µs | maxVal = ");
    Serial.println(maxVal);
  }
}


// Print intensitas dengan label wavelength
void printIntensity(uint16_t* data) {
  for (int i = 0; i < PIXEL_COUNT; i++) {
    Serial.print(pixelToWavelength(i + PIXEL_START), 1);
    Serial.print(",");
    Serial.println(data[i]);
  }
}

// Print absorbansi dengan label wavelength (CSV)
void printAbsorbance() {
  for (int i = 0; i < PIXEL_COUNT; i++) {
    Serial.print(pixelToWavelength(i + PIXEL_START), 1);
    Serial.print(",");
    Serial.println(Absorbance[i], 4);
  }
}