#ifndef RUNTIME_UI_H
#define RUNTIME_UI_H

#include <Arduino.h>
#include <lvgl.h>

#include "app_shared.h"

namespace runtime_ui {

void begin(lv_obj_t *parent);
void updateMonitor(const app::ControllerStatus &status, size_t scheduleCount);
void tickLoading(const app::ControllerStatus &status);

}  // namespace runtime_ui

#endif
