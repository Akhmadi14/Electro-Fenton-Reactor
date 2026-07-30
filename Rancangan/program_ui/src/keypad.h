#pragma once

#include <lvgl.h>

// External 4x4 matrix keypad on PCF8574 (I2C @ 0x20).
// Shares the same Wire bus already initialised by the GT911 touch driver
// (SDA=19, SCL=20). Do NOT call Wire.begin() here.
//
// Behaviour (per spec):
//   0-9 -> append digit
//   *   -> append '.'
//   D   -> backspace (delete last char)
//   A, B, C, # -> ignored
//
// When no textarea is focused, tick() is a no-op (no I2C traffic).
namespace keypad {

void begin();
void tick(lv_obj_t *active_textarea);

}  // namespace keypad
