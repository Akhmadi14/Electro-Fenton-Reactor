#include "keypad.h"

#include <Arduino.h>
#include <Wire.h>

// Reference (working on bare ESP32) provided by user:
//   row mask  | byte read when key pressed -> logical key
//   0xEF      | 0xEE 0xED 0xEB 0xE7         -> 1 2 3 A
//   0xDF      | 0xDE 0xDD 0xDB 0xD7         -> 4 5 6 B
//   0xBF      | 0xBE 0xBD 0xBB 0xB7         -> 7 8 9 C
//   0x7F      | 0x7E 0x7D 0x7B 0x77         -> * 0 # D
//
// The PCF8574 is wired so the upper nibble drives rows (active-low) and
// the lower nibble reads columns (pulled high, pressed key pulls low).

namespace {

constexpr uint8_t kPcfAddress = 0x20;
constexpr uint32_t kDebounceMs = 300;
constexpr uint32_t kScanIntervalMs = 30;

bool gButtonPressed = false;
uint32_t gButtonTimer = 0;
uint32_t gLastScanMs = 0;
uint8_t gRowIndex = 0;

struct RowMap {
  uint8_t mask;
  uint8_t code[4];   // PCF8574 byte when each column key is pressed
  char physical[4];  // logical key per column
};

const RowMap kRows[] = {
  {0xEF, {0xEE, 0xED, 0xEB, 0xE7}, {'1', '2', '3', 'A'}},
  {0xDF, {0xDE, 0xDD, 0xDB, 0xD7}, {'4', '5', '6', 'B'}},
  {0xBF, {0xBE, 0xBD, 0xBB, 0xB7}, {'7', '8', '9', 'C'}},
  {0x7F, {0x7E, 0x7D, 0x7B, 0x77}, {'*', '0', '#', 'D'}},
};

// Returns -1 if the device did not respond.
int scanRow(uint8_t mask) {
  Wire.beginTransmission(kPcfAddress);
  Wire.write(mask);
  if (Wire.endTransmission() != 0) {
    return -1;
  }
  if (Wire.requestFrom(static_cast<int>(kPcfAddress), 1) != 1) {
    return -1;
  }
  return Wire.read();
}

void dispatch(lv_obj_t *ta, char key) {
  switch (key) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      lv_textarea_add_char(ta, static_cast<uint32_t>(key));
      // === DEBUG: KEYPAD (hapus saat produksi) ===
      //Serial.print("[KEYPAD] '"); Serial.print(key); Serial.println("'");
      // === END DEBUG ===
      break;

    case '*':
      lv_textarea_add_char(ta, static_cast<uint32_t>('.'));
      // === DEBUG: KEYPAD (hapus saat produksi) ===
      //Serial.println("[KEYPAD] '.'");
      // === END DEBUG ===
      break;

    case 'D':
      lv_textarea_del_char(ta);
      // === DEBUG: KEYPAD (hapus saat produksi) ===
      //Serial.println("[KEYPAD] BACKSPACE");
      // === END DEBUG ===
      break;

    default:
      // A, B, C, '#' -> diabaikan per spec.
      // === DEBUG: KEYPAD (hapus saat produksi) ===
      //Serial.print("[KEYPAD] (ignored) '"); Serial.print(key); Serial.println("'");
      // === END DEBUG ===
      break;
  }
}

}  // namespace

namespace keypad {

void begin() {
  // === DEBUG: KEYPAD (hapus saat produksi) ===
  //Serial.println("[KEYPAD] ready (PCF8574 @ 0x20, shared Wire bus)");
  // === END DEBUG ===
}

void tick(lv_obj_t *active_textarea) {
  if (active_textarea == nullptr) {
    return;
  }

  if (gButtonPressed) {
    if (millis() - gButtonTimer >= kDebounceMs) {
      gButtonPressed = false;
    } else {
      return;
    }
  }

  const uint32_t now = millis();
  if (now - gLastScanMs < kScanIntervalMs) {
    return;
  }
  gLastScanMs = now;

  const RowMap &row = kRows[gRowIndex];
  gRowIndex = static_cast<uint8_t>((gRowIndex + 1) % 4);

  int read = scanRow(row.mask);
  if (read < 0) {
    return;
  }
  const uint8_t byte = static_cast<uint8_t>(read);
  for (int col = 0; col < 4; ++col) {
    if (byte == row.code[col]) {
      dispatch(active_textarea, row.physical[col]);
      gButtonPressed = true;
      gButtonTimer = now;
      return;  // one key per scan
    }
  }
}

}  // namespace keypad
