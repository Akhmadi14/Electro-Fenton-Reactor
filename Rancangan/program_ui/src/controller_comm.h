#ifndef CONTROLLER_COMM_H
#define CONTROLLER_COMM_H

#include <Arduino.h>

#include "app_shared.h"

namespace controller_comm {

struct Config {
  uint32_t baudRate = 115200;
  int rxPin = 44;
  int txPin = 43;
  uint32_t retryIntervalMs = 1500;
  uint32_t rxIdleFlushMs = 25;
};

void begin(HardwareSerial &serial, const Config &config);
void tick();
bool startSchedule(const String &payload);
bool startInisialisasi(const String &payload);
bool startFlushing();
void requestForceStop();
const app::ControllerStatus &status();
bool consumeDirtyFlag();

}  // namespace controller_comm

#endif
