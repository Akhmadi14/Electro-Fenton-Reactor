 /*
 * ============================================================
 * Closed-Loop PID RPM Control - Motor BLDC (ESP32)
 * ============================================================
 * Platform  : ESP32 (Arduino Core 3.x)
 * PWM Pin   : GPIO 27   (output ke driver motor)
 * Hall Pin  : GPIO 32   (input dari Hall sensor)
 *
 * Prinsip Hall sensor:
 *   - LOW  = magnet dekat sensor
 *   - HIGH = magnet menjauh
 *   -> RISING edge (LOW->HIGH) = 1 putaran penuh
 *
 * Sampling : 20 ms (presisi via micros(), bukan delay)
 * Output Serial (CSV):
 *   targetrpm,rpmterbaca
 *   (baris dimulai '#' adalah komentar, bukan data)
 * ============================================================
 */

// ── Pin ──────────────────────────────────────────────────────
const int PWM_PIN  = 27;
const int HALL_PIN = 32;

// ── Konfigurasi PWM ──────────────────────────────────────────
const int PWM_FREQ       = 5000;   // frekuensi carrier PWM (Hz)
const int PWM_RESOLUTION = 8;      // resolusi 8-bit -> nilai 0-255

// ── Konfigurasi Waktu ────────────────────────────────────────
const unsigned long START_DELAY_MS     = 3000UL;  // delay awal (ms)
const unsigned long SAMPLE_INTERVAL_US = 20000UL; // 20 ms dalam us
const float         SAMPLE_INTERVAL_S  = 0.02f;   // 20 ms dalam detik

// ── PID Gain (tuning sesuai motor) ───────────────────────────
const float KP = 0.005f;
const float KI = 0.046f;
//const float KD = 0.0f;
const float INTEGRAL_LIMIT = 5000.0f;

// ── Batas PWM ────────────────────────────────────────────────
const int PWM_MIN = 0;
const int PWM_MAX = 250; // ubah jika ingin batas atas berbeda

// ── Target RPM (ubah di sini) ────────────────────────────────
float g_target_rpm = 300.0f;

// ── Serial Command (opsional, mudah dihapus) ─────────────────
#define ENABLE_SERIAL_COMMANDS 1
#define CMD_BUF_LEN 32
static bool     g_streaming_enabled = true;
static char     g_cmd_buf[CMD_BUF_LEN];
static uint8_t  g_cmd_len = 0;

// ── Variabel volatile (diakses ISR dan main loop) ────────────
// Metode: period measurement - ISR mencatat timestamp setiap rising edge.
// RPM dihitung dari selisih waktu antar dua pulsa terakhir.
volatile unsigned long g_lastPulse_us = 0;  // waktu rising edge terakhir (us)
volatile unsigned long g_period_us    = 0;  // periode antar 2 pulsa terakhir (us)

// Timeout: jika tidak ada pulsa selama ini, anggap RPM = 0
const unsigned long RPM_TIMEOUT_US = 500000UL; // 500 ms

// ── ISR Hall Sensor ──────────────────────────────────────────
void IRAM_ATTR hallISR() {
  unsigned long now = micros();
  if (g_lastPulse_us != 0) {
    g_period_us = now - g_lastPulse_us;
  }
  g_lastPulse_us = now;
}

#if ENABLE_SERIAL_COMMANDS
static void parseCommand(char *cmd) {
  if (strcmp(cmd, "s") == 0) {
    g_streaming_enabled = false;
    Serial.println(F("# STREAM PAUSE"));
    return;
  }
  if (strcmp(cmd, "start") == 0) {
    g_streaming_enabled = true;
    Serial.println(F("# STREAM START"));
    return;
  }
  if (strncmp(cmd, "rpm=", 4) == 0) {
    float val = (float)atof(cmd + 4);
    if (val < 0.0f) {
      val = 0.0f;
    }
    g_target_rpm = val;
    Serial.print(F("# TARGET RPM = "));
    Serial.println(g_target_rpm, 1);
  }
}

static void handleSerial() {
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (g_cmd_len == 0) {
        continue;
      }
      g_cmd_buf[g_cmd_len] = '\0';
      for (uint8_t i = 0; i < g_cmd_len; i++) {
        if (g_cmd_buf[i] >= 'A' && g_cmd_buf[i] <= 'Z') {
          g_cmd_buf[i] = (char)(g_cmd_buf[i] + ('a' - 'A'));
        }
      }
      parseCommand(g_cmd_buf);
      g_cmd_len = 0;
    } else if (g_cmd_len < (CMD_BUF_LEN - 1)) {
      g_cmd_buf[g_cmd_len++] = c;
    }
  }
}
#endif

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Inisialisasi PWM output
  ledcAttach(PWM_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcWrite(PWM_PIN, 0);

  // Inisialisasi Hall sensor input + pasang interrupt RISING edge
  pinMode(HALL_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), hallISR, RISING);

  // ── Countdown 3 detik ──────────────────────────────────────
  Serial.println(F("# ================================================"));
  Serial.println(F("# Closed-loop PID RPM control"));
  Serial.println(F("# ================================================"));
#if ENABLE_SERIAL_COMMANDS
  Serial.println(F("# Cmd: s (pause), start (resume), rpm=XXX"));
#endif
  for (int i = (int)(START_DELAY_MS / 1000); i > 0; i--) {
    Serial.print(F("# Mulai dalam "));
    Serial.print(i);
    Serial.println(F(" detik..."));
    delay(1000);
  }
  Serial.println(F("# CONTROL START"));
  Serial.println(F("# Format: targetrpm,rpmterbaca"));
  Serial.println(F("targetrpm,rpmterbaca"));

  // ── Reset variabel period sebelum kontrol ─────────────────
  noInterrupts();
  g_lastPulse_us = 0;
  g_period_us = 0;
  interrupts();

  unsigned long nextSample_us = micros();
  int currentPWM = -1;
  float integral = 0.0f;
  float prev_error = 0.0f;

  // ─────────────────────────────────────────────────────────
  // Loop kontrol utama (20 ms)
  // ─────────────────────────────────────────────────────────
  while (true) {
    while ((long)(micros() - nextSample_us) < 0) {
      // busy-wait
    }

#if ENABLE_SERIAL_COMMANDS
    handleSerial();
#endif

    // ── Baca period & timestamp terakhir secara atomic ───────
    noInterrupts();
    unsigned long period = g_period_us;
    unsigned long lastPulse = g_lastPulse_us;
    interrupts();

    // ── Hitung RPM dari periode antar pulsa ─────────────────
    float rpm = 0.0f;
    unsigned long now_us = micros();
    bool motorStopped = (lastPulse == 0) || ((now_us - lastPulse) > RPM_TIMEOUT_US);
    if (!motorStopped && period > 0) {
      rpm = 60000000.0f / (float)period;
    }

    // ── PID Controller ─────────────────────────────────────-
    float pwm_cmd_f = 0.0f;
    if (g_target_rpm > 0.0f) {
      float error = g_target_rpm - rpm;
      integral += error * SAMPLE_INTERVAL_S;
      if (integral > INTEGRAL_LIMIT) {
        integral = INTEGRAL_LIMIT;
      } else if (integral < -INTEGRAL_LIMIT) {
        integral = -INTEGRAL_LIMIT;
      }
      //Pengendali PI Kd diabaikan
      //float derivative = (error - prev_error) / SAMPLE_INTERVAL_S;
      prev_error = error;
      pwm_cmd_f = (KP * error) + (KI * integral); //+ (KD * derivative);
    } else {
      integral = 0.0f;
      prev_error = 0.0f;
      pwm_cmd_f = 0.0f;
    }

    int pwm_cmd = (int)pwm_cmd_f;
    if (pwm_cmd < PWM_MIN) {
      pwm_cmd = PWM_MIN;
    } else if (pwm_cmd > PWM_MAX) {
      pwm_cmd = PWM_MAX;
    }
    if (pwm_cmd != currentPWM) {
      currentPWM = pwm_cmd;
      ledcWrite(PWM_PIN, currentPWM);
    }

    // ── Output CSV ─────────────────────────────────────────-
    if (g_streaming_enabled) {
      Serial.print((unsigned long)(g_target_rpm + 0.5f));
      Serial.print(',');
      Serial.println((unsigned long)(rpm + 0.5f));
    }

    nextSample_us += SAMPLE_INTERVAL_US;
  }
}

// loop() tidak dipakai — semua logika ada di setup()
void loop() {}
