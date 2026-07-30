#ifndef INIT_FLUSHING_UI_H
#define INIT_FLUSHING_UI_H

#include <Arduino.h>

#include "app_shared.h"

namespace init_flushing_ui {

void begin();
void syncMenuButtons(const app::ControllerStatus &status);
void syncInisialisasiScreen(const app::ControllerStatus &status);
void syncFlushingScreen(const app::ControllerStatus &status);
void tickLoading(const app::ControllerStatus &status);
bool validateAndStartInisialisasi();
bool requestFlushing();

}  // namespace init_flushing_ui

#endif
