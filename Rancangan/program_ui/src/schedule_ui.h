#ifndef SCHEDULE_UI_H
#define SCHEDULE_UI_H

#include <Arduino.h>

#include "app_shared.h"

namespace schedule_ui {

void begin();
void sync(const app::ControllerStatus &status);
void hideWarningBoxes();
bool addElectroFenton();
bool addAdsorpsi();
bool addSpektrofotometri();
bool startSend();
size_t count();
void resetSchedule();

void nextPage();
void backPage();

// Read-only access to schedule entries for persistence/export.
size_t entryCount();
const app::ScheduleEntry &entryAt(size_t index);

}  // namespace schedule_ui

#endif
