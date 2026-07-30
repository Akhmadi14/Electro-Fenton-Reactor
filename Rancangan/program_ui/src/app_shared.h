#ifndef APP_SHARED_H
#define APP_SHARED_H

#include <Arduino.h>

namespace app {

enum class ReactionType {
  None,
  ElectroFenton,
  Adsorpsi,
  Spektrofotometri
};

enum class ControllerState {
  InisialisasiInput,
  InisialisasiEksekusi,
  Idle,
  Execution,
  Flushing
};

enum class PendingCommandType {
  None,
  StartSchedule,
  ForceStop,
  StartInisialisasi,
  StartFlushing
};

struct ScheduleEntry {
  ReactionType reaction = ReactionType::None;
  float suhu = 0.0f;
  long rpm = 0;
  long arus = 0;
  long durasiDetik = 0;
  long airflowLevel = 0;
  bool ukurDegradasi = false;
};

struct ExecutionSnapshot {
  ReactionType reaction = ReactionType::None;
  float suhu = 0.0f;
  float pH = 0.0f;
  //oat gas = 0.0f;
  String gas = "";
  long rpm = 0;
  long arus = 0;
  long airflowLevel = 0;
  long waktuTersisa = 0;
  bool ukurDegradasi = false;
  bool hasSuhu = false;
  bool hasPH = false;
  bool hasGas = false;
  bool hasRpm = false;
  bool hasArus = false;
  bool hasAirflow = false;
  bool hasWaktuTersisa = false;
  bool hasUkurDegradasi = false;
};

struct InisialisasiStatus {
  int sampelPercent = 0;
  int h2so4Status = 0;   // -1 = skip, 0 = in-progress, 1 = done
  int feso4Status = 0;
  int kalibrasiStatus = 0;
  int prefillStatus = -1; // -1 = tidak prefill, 0 = sedang, 1 = selesai
  bool done = false;
};

struct FlushingStatus {
  int progress = 0;
  bool done = false;
};

struct ControllerStatus {
  ControllerState state = ControllerState::InisialisasiInput;
  ExecutionSnapshot execution;
  InisialisasiStatus inisialisasi;
  FlushingStatus flushing;
  PendingCommandType pendingType = PendingCommandType::None;
  const char *expectedAck = nullptr;
  bool waitingAck = false;
  uint32_t sendCount = 0;
};

const char *reactionToJsonName(ReactionType reaction);
const char *reactionToDisplayName(ReactionType reaction);
ReactionType reactionFromString(String value);
String formatFloatValue(float value);
String formatDurationHhMmSs(long totalSeconds);

}  // namespace app

#endif
