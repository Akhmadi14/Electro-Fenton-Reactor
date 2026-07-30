#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Inisialisasi object ADS1115
Adafruit_ADS1115 ads; 

// Konfigurasi Pin ESP32
const int dacPin = 25;           // GPIO 25 (DAC1)
float targetVoltage = 0.2;       // Nilai default: 0.2V

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("--- Kontrol DAC ESP32 & Pembacaan ADS1115 ---");

  // Inisialisasi modul ADS1115
  // Secara default pada ESP32, SDA = GPIO 21 dan SCL = GPIO 22
  if (!ads.begin()) {
    Serial.println("Gagal menemukan modul ADS1115. Cek koneksi kabel I2C!");
    while (1); // Sistem berhenti di sini jika sensor tidak terdeteksi
  }

  // Mengatur Gain (Penguatan) PGA pada ADS1115
  // Pilih gain yang sesuai dengan target maksimal tegangan yang masuk ke A0:
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  (1 bit = 0.1875mV)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  (1 bit = 0.125mV)
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  (1 bit = 0.0625mV)
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  (1 bit = 0.03125mV)
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  (1 bit = 0.015625mV)
  
  // Menggunakan Gain 16x (Sangat presisi untuk membaca shunt resistor 200mV)
  ads.setGain(GAIN_SIXTEEN);       // 16x gain  +/- 0.256V  (1 bit = 0.0078125mV)

  Serial.println("ADS1115 Siap!");
  Serial.println("Masukkan tegangan target DAC (0.0 - 3.3) di Serial Monitor:");
  Serial.print("Tegangan saat ini (Default): ");
  Serial.print(targetVoltage);
  Serial.println(" V\n");
}

void loop() {
  // ==========================================
  // 1. BLOK KONTROL DAC (Input Serial Monitor)
  // ==========================================
  if (Serial.available() > 0) {
    float input = Serial.parseFloat();
    
    // Validasi input: Pastikan antara 0V hingga 3.3V
    if (input >= 0.0 && input <= 3.3) {
      targetVoltage = input;
      Serial.print(">> Tegangan DAC diubah ke: ");
      Serial.print(targetVoltage);
      Serial.println(" V");
    } else {
      Serial.println("!! Error: Masukkan nilai antara 0.0 sampai 3.3 !!");
    }
    
    // Membersihkan buffer serial
    while(Serial.available() > 0) Serial.read();
  }

  // Menghitung dan mengeluarkan nilai DAC (8-bit: 0-255)
  int dacValue = (targetVoltage / 3.3) * 255;
  dacWrite(dacPin, dacValue);


  // ==========================================
  // 2. BLOK PEMBACAAN SENSOR ADS1115
  // ==========================================
  
  // Membaca nilai ADC mentah (16-bit) dari channel A0 secara Single-Ended
  int16_t adc0 = ads.readADC_SingleEnded(0);
  
  // Mengkonversi nilai mentah menjadi tegangan (Volt) secara otomatis
  float volts0 = ads.computeVolts(adc0);


  // ==========================================
  // 3. BLOK TAMPILAN INFORMASI
  // ==========================================
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 750) { // Update tampilan setiap 2 detik
    Serial.println("----------------------------------------");
    
    // Tampilan Output DAC
    Serial.print("[OUT] DAC Pin 25 : "); 
    Serial.print(targetVoltage); 
    Serial.print(" V (Nilai 8-bit: "); 
    Serial.print(dacValue);
    Serial.println(")");
    
    // Tampilan Input ADS1115
    Serial.print("[IN]  ADS1115 A0 : "); 
    // Menampilkan 4 angka di belakang koma (misal: 0.1234 V) untuk presisi ekstra
    Serial.print(volts0, 4); 
    Serial.print(" V (Data Mentah: "); 
    Serial.print(adc0);
    Serial.println(")");
    
    lastPrint = millis();
  }
}