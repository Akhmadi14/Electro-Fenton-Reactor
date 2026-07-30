#include "controller_comm.h"

#include <ArduinoJson.h>

#include "schedule_ui.h"
#include "spektro_ui.h"

namespace {

struct PendingCommand {
  app::PendingCommandType type = app::PendingCommandType::None;
  String payload;
  const char *expectedAck = nullptr;
  bool waiting = false;
  uint32_t lastSendAtMs = 0;
  uint32_t sendCount = 0;
};

HardwareSerial *controllerSerial = nullptr;
controller_comm::Config activeConfig;
app::ControllerStatus controllerStatus;
PendingCommand pendingCommand;
String rxBuffer;
uint32_t lastControllerByteMs = 0;
bool dirtyFlag = true;

String trimCopy(String value) {
  value.trim();
  return value;
}

void markDirty() {
  dirtyFlag = true;
}

void clearExecutionSnapshot() {
  controllerStatus.execution = app::ExecutionSnapshot();
}

bool hasExecutionSnapshotData() {
  const app::ExecutionSnapshot &snapshot = controllerStatus.execution;
  return snapshot.reaction != app::ReactionType::None ||
         snapshot.hasSuhu ||
         snapshot.hasPH ||
         snapshot.hasGas ||
         snapshot.hasRpm ||
         snapshot.hasArus ||
         snapshot.hasAirflow ||
         snapshot.hasWaktuTersisa ||
         snapshot.hasUkurDegradasi;
}

void syncPendingStatus() {
  controllerStatus.pendingType = pendingCommand.type;
  controllerStatus.expectedAck = pendingCommand.expectedAck;
  controllerStatus.waitingAck = pendingCommand.waiting;
  controllerStatus.sendCount = pendingCommand.sendCount;
}

void sendControllerMessage(const String &message) {
  if (!controllerSerial) {
    return;
  }

  controllerSerial->println(message);
  controllerSerial->flush();

  Serial.print("[UART->CTRL] ");
  Serial.println(message);
}

void clearPendingCommand() {
  pendingCommand = PendingCommand();
  syncPendingStatus();
  markDirty();
}

void sendPendingCommandNow() {
  if (!pendingCommand.waiting || pendingCommand.payload.length() == 0 || !controllerSerial) {
    return;
  }

  ++pendingCommand.sendCount;
  pendingCommand.lastSendAtMs = millis();
  syncPendingStatus();
  sendControllerMessage(pendingCommand.payload);
  markDirty();
}

void queuePendingCommand(app::PendingCommandType type,
                         const String &payload,
                         const char *expectedAck) {
  pendingCommand.type = type;
  pendingCommand.payload = payload;
  pendingCommand.expectedAck = expectedAck;
  pendingCommand.waiting = true;
  pendingCommand.lastSendAtMs = 0;
  pendingCommand.sendCount = 0;
  syncPendingStatus();
  sendPendingCommandNow();
}

void maybeRetryPendingCommand() {
  if (!pendingCommand.waiting || !controllerSerial) {
    return;
  }

  if (millis() - pendingCommand.lastSendAtMs < activeConfig.retryIntervalMs) {
    return;
  }

  sendPendingCommandNow();
}

bool looksLikeCompleteMessage(const String &message) {
  if (message.length() == 0) {
    return false;
  }

  if (!message.startsWith("{")) {
    return true;
  }

  return message.endsWith("}");
}

JsonVariantConst getExecutionField(const JsonDocument &doc, const char *key) {
  JsonVariantConst parameter = doc["parameter"];
  if (!parameter.isNull()) {
    JsonVariantConst nestedField = parameter[key];
    if (!nestedField.isNull()) {
      return nestedField;
    }
  }

  return doc[key];
}

template <typename T>
bool readExecutionField(const JsonDocument &doc, const char *key, T &value) {
  JsonVariantConst field = getExecutionField(doc, key);
  if (field.isNull()) {
    return false;
  }

  value = field.as<T>();
  return true;
}

void applyStateMessage(const JsonDocument &doc) {
  const String stateValue = doc["State"] | "";

  if (stateValue == "Idle") {
    if (controllerStatus.state == app::ControllerState::InisialisasiInput ||
        controllerStatus.state == app::ControllerState::InisialisasiEksekusi ||
        controllerStatus.state == app::ControllerState::Flushing) {
      return;
    }

    if (controllerStatus.state == app::ControllerState::Idle &&
        !hasExecutionSnapshotData()) {
      return;
    }

    controllerStatus.state = app::ControllerState::Idle;
    clearExecutionSnapshot();
    markDirty();
    return;
  }

  if (stateValue != "Eksekusi") {
    return;
  }

  const String reactionValue = doc["reaksi"] | "";
  const int procId = doc["id"] | 0;

  // --- Handle spectro measurement before state guard (no snapshot needed) ---
  if (reactionValue == "spektro") {
    controllerStatus.state = app::ControllerState::Execution;
    JsonArrayConst spektrumArr = doc["spektrum"].as<JsonArrayConst>();
    if (procId > 0 && !spektrumArr.isNull()) {
      static float tmpSpektrum[128];
      int cnt = 0;
      for (JsonVariantConst v : spektrumArr) {
        if (cnt >= 128) break;
        tmpSpektrum[cnt++] = v.as<float>();
      }
      spektro_ui::onSpektrumReceived(procId, tmpSpektrum, cnt);
    }
    markDirty();
    return;
  }

  if (controllerStatus.state != app::ControllerState::Idle &&
      controllerStatus.state != app::ControllerState::Execution) {
    return;
  }

  app::ExecutionSnapshot snapshot;
  snapshot.hasSuhu = readExecutionField<float>(doc, "suhu", snapshot.suhu);
  snapshot.hasPH = readExecutionField<float>(doc, "pH", snapshot.pH) ||
                   readExecutionField<float>(doc, "ph", snapshot.pH);
  snapshot.hasGas = readExecutionField<String>(doc, "gas", snapshot.gas);
  snapshot.hasRpm = readExecutionField<long>(doc, "rpm", snapshot.rpm);
  snapshot.hasArus = readExecutionField<long>(doc, "arus", snapshot.arus);
  snapshot.hasAirflow = readExecutionField<long>(doc, "airflow_level", snapshot.airflowLevel);
  snapshot.hasWaktuTersisa = readExecutionField<long>(doc, "waktu_tersisa", snapshot.waktuTersisa);
  snapshot.hasUkurDegradasi = readExecutionField<bool>(doc, "ukur_degradasi", snapshot.ukurDegradasi);

  if (reactionValue.length() > 0) {
    snapshot.reaction = app::reactionFromString(reactionValue);
  }

  controllerStatus.execution = snapshot;
  controllerStatus.state = app::ControllerState::Execution;

  // --- Parse degradasi measurement if present ---
  if (procId > 0) {
    JsonObjectConst degradasiObj = doc["update_degradasi"].as<JsonObjectConst>();
    if (!degradasiObj.isNull()) {
      int    t_menit    = degradasiObj["t_menit"]   | 0;
      float  absorbansi = degradasiObj["absorbansi"] | 0.0f;
      spektro_ui::onDegradasiPoint(procId, snapshot.reaction,
                                   t_menit, absorbansi);
    }
  }

  markDirty();
}

void applyStatusFieldMessage(const JsonDocument &doc, const String &statusValue) {
  if (statusValue == "ongoing" || statusValue == "done") {
    controllerStatus.inisialisasi.sampelPercent = doc["sampel"] | 0;
    controllerStatus.inisialisasi.h2so4Status = doc["h2so4"] | 0;
    controllerStatus.inisialisasi.feso4Status = doc["feso4"] | 0;
    controllerStatus.inisialisasi.kalibrasiStatus = doc["kalibrasi"] | 0;
    controllerStatus.inisialisasi.prefillStatus    = doc["prefill"]   | -1;
    
    if (statusValue == "done") {
      controllerStatus.inisialisasi.done = true;
      sendControllerMessage("ACK_DONE");
      controllerStatus.state = app::ControllerState::Idle;
      controllerStatus.flushing = app::FlushingStatus();
    }
    markDirty();
    return;
  }

  if (statusValue == "flushing" && doc["progress"].is<int>()) {
    controllerStatus.flushing.progress = doc["progress"] | 0;
    markDirty();
    return;
  }

  if (statusValue == "flushing_done") {
    controllerStatus.flushing.progress = doc["progress"] | 100;
    controllerStatus.flushing.done = true;
    sendControllerMessage("ACK_FLUSHINGDONE");
    controllerStatus.state = app::ControllerState::InisialisasiInput;
    controllerStatus.inisialisasi = app::InisialisasiStatus();
    spektro_ui::resetAll();
    schedule_ui::resetSchedule();
    markDirty();
    return;
  }
}

void handleExecuteDone() {
  sendControllerMessage("ACK_EXECUTE_DONE");
  controllerStatus.state = app::ControllerState::Idle;
  clearExecutionSnapshot();
  markDirty();
}

void handleControllerMessage(String message) {
  message.trim();
  if (message.length() == 0) {
    return;
  }

  Serial.print("[UART<-CTRL] ");
  Serial.println(message);

  if (pendingCommand.waiting && pendingCommand.expectedAck &&
      message == pendingCommand.expectedAck) {
    const app::PendingCommandType acknowledgedType = pendingCommand.type;
    const String acknowledgedPayload = pendingCommand.payload;
    clearPendingCommand();

    if (acknowledgedType == app::PendingCommandType::ForceStop) {
      controllerStatus.state = app::ControllerState::Idle;
      clearExecutionSnapshot();
      controllerStatus.flushing = app::FlushingStatus();
      markDirty();
    } else if (acknowledgedType == app::PendingCommandType::StartInisialisasi) {
      controllerStatus.state = app::ControllerState::InisialisasiEksekusi;

      app::InisialisasiStatus initStatus;
      JsonDocument sentDoc;
      if (!deserializeJson(sentDoc, acknowledgedPayload)) {
        float sentPh = sentDoc["ph"] | 0.0f;
        float sentFe = sentDoc["fe"] | 0.0f;
        int sentKal  = sentDoc["kalibrasi"]  | 0;
        int sentPref = sentDoc["prefilling"]  | -1;
        initStatus.h2so4Status     = (sentPh < 0.0f) ? -1 : 0;
        initStatus.feso4Status     = (sentFe < 0.0f) ? -1 : 0;
        initStatus.kalibrasiStatus = (sentKal  == 1) ? 0 : -1;
        initStatus.prefillStatus   = (sentPref == 1) ? 0 : -1;
      }
      controllerStatus.inisialisasi = initStatus;
      markDirty();
    } else if (acknowledgedType == app::PendingCommandType::StartFlushing) {
      controllerStatus.state = app::ControllerState::Flushing;
      controllerStatus.flushing = app::FlushingStatus();
      markDirty();
    }
    return;
  }

  if (message == "EXECUTE_DONE") {
    handleExecuteDone();
    return;
  }

  if (message.startsWith("{") && message.endsWith("}")) {
    JsonDocument doc;
    if (deserializeJson(doc, message)) {
      return;
    }

    const String statusValue = doc["status"] | "";
    if (statusValue.length() > 0) {
      applyStatusFieldMessage(doc, statusValue);
      return;
    }

    applyStateMessage(doc);
  }
}

void processControllerUart() {
  if (!controllerSerial) {
    return;
  }

  while (controllerSerial->available()) {
    const char incoming = static_cast<char>(controllerSerial->read());
    lastControllerByteMs = millis();

    if (incoming == '\r') {
      continue;
    }

    if (incoming == '\n') {
      String message = trimCopy(rxBuffer);
      rxBuffer = "";
      if (message.length() > 0) {
        handleControllerMessage(message);
      }
      continue;
    }

    rxBuffer += incoming;
  }

  if (rxBuffer.length() == 0) {
    return;
  }

  if (millis() - lastControllerByteMs < activeConfig.rxIdleFlushMs) {
    return;
  }

  String message = trimCopy(rxBuffer);
  if (!looksLikeCompleteMessage(message)) {
    return;
  }

  rxBuffer = "";
  handleControllerMessage(message);
}

}  // namespace

namespace controller_comm {

void begin(HardwareSerial &serial, const Config &config) {
  controllerSerial = &serial;
  activeConfig = config;
  controllerSerial->begin(activeConfig.baudRate, SERIAL_8N1, activeConfig.rxPin, activeConfig.txPin);

  controllerStatus = app::ControllerStatus();
  clearExecutionSnapshot();
  clearPendingCommand();
  rxBuffer.reserve(1500);  // sufficient for 128-element spektrum JSON (~1.2 kB)
  markDirty();
}

void tick() {
  processControllerUart();
  maybeRetryPendingCommand();
}

bool startSchedule(const String &payload) {
  if (payload.length() == 0) {
    return false;
  }

  if (pendingCommand.waiting) {
    if (pendingCommand.type == app::PendingCommandType::StartSchedule) {
      pendingCommand.payload = payload;
      sendPendingCommandNow();
      return true;
    }
    return false;
  }

  queuePendingCommand(app::PendingCommandType::StartSchedule, payload, "ACK_START");
  return true;
}

bool startInisialisasi(const String &payload) {
  if (payload.length() == 0) {
    return false;
  }

  if (pendingCommand.waiting) {
    if (pendingCommand.type == app::PendingCommandType::StartInisialisasi) {
      pendingCommand.payload = payload;
      sendPendingCommandNow();
      return true;
    }
    return false;
  }

  queuePendingCommand(app::PendingCommandType::StartInisialisasi, payload, "ACK_START");
  return true;
}

bool startFlushing() {
  if (pendingCommand.waiting) {
    return false;
  }

  queuePendingCommand(app::PendingCommandType::StartFlushing,
                      "{\"status\":\"flushing\"}", "ACK_FLUSHING");
  return true;
}

void requestForceStop() {
  queuePendingCommand(app::PendingCommandType::ForceStop, "FORCE_STOP", "ACK:FORCE_STOP");
}

const app::ControllerStatus &status() {
  return controllerStatus;
}

bool consumeDirtyFlag() {
  const bool wasDirty = dirtyFlag;
  dirtyFlag = false;
  return wasDirty;
}

}  // namespace controller_comm
