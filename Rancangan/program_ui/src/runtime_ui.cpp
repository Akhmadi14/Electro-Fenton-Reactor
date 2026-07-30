#include "runtime_ui.h"

namespace {

constexpr uint32_t kLoadingTickMs = 100;

struct RuntimeMonitorWidgets {
  lv_obj_t *processValue = nullptr;
  lv_obj_t *detailLabel = nullptr;
  lv_obj_t *loadingLabel = nullptr;
  lv_obj_t *loadingBar = nullptr;
};

RuntimeMonitorWidgets widgets;
uint32_t lastLoadingTickMs = 0;
int loadingBarValue = 0;

void setHidden(lv_obj_t *obj, bool hidden) {
  if (!obj) {
    return;
  }

  if (hidden) {
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

String buildExecutionDetailText(const app::ExecutionSnapshot &snapshot) {
  String text;

  if (snapshot.hasSuhu) {
    text += "Suhu: ";
    text += app::formatFloatValue(snapshot.suhu);
    text += " C";
  }

  if (snapshot.hasPH) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "pH: ";
    text += app::formatFloatValue(snapshot.pH);
  }

  if (snapshot.hasGas) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "Gas: ";
    text += String(snapshot.gas);
  }

  if (snapshot.hasRpm) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "RPM: ";
    text += String(snapshot.rpm);
  }

  if (snapshot.hasArus) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "Arus: ";
    text += String(snapshot.arus);
    text += " mA";
  }

  if (snapshot.hasAirflow) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "Airflow level: ";
    text += String(snapshot.airflowLevel);
  }

  if (snapshot.hasUkurDegradasi) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "Ukur degradasi: ";
    text += snapshot.ukurDegradasi ? "Ya" : "Tidak";
  }

  if (snapshot.hasWaktuTersisa) {
    if (text.length() > 0) {
      text += "\n";
    }
    text += "Waktu tersisa: ";
    text += app::formatDurationHhMmSs(snapshot.waktuTersisa);
  }

  return text;
}

String buildWaitingDetailText(const app::ControllerStatus &status, size_t scheduleCount) {
  if (status.pendingType == app::PendingCommandType::StartSchedule) {
    String text = "Mengirim jadwal ke controller.\n";
    text += "Jumlah proses: ";
    text += String(scheduleCount);
    text += "\nJSON akan dikirim ulang bila ACK belum diterima.";
    return text;
  }

  if (status.pendingType == app::PendingCommandType::ForceStop) {
    return String("FORCE_STOP dikirim.\nPerintah akan diulang sampai controller mengirim ACK.");
  }

  return "";
}

String buildWaitingStatusText(const app::ControllerStatus &status) {
  if (!status.waitingAck || !status.expectedAck) {
    return "";
  }

  String text = "Menunggu ";
  text += status.expectedAck;
  text += " | kirim ke-";
  text += String(status.sendCount);
  return text;
}

}  // namespace

namespace runtime_ui {

void begin(lv_obj_t *parent) {
  if (widgets.processValue || !parent) {
    return;
  }

  widgets.processValue = lv_label_create(parent);
  lv_obj_set_pos(widgets.processValue, 235, 52);
  lv_obj_set_size(widgets.processValue, 380, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(widgets.processValue, &lv_font_montserrat_30,
                             LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(widgets.processValue, lv_color_hex(0xff1565c0),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(widgets.processValue, "");

  widgets.detailLabel = lv_label_create(parent);
  lv_obj_set_pos(widgets.detailLabel, 44, 120);
  lv_obj_set_size(widgets.detailLabel, 600, LV_SIZE_CONTENT);
  lv_label_set_long_mode(widgets.detailLabel, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_font(widgets.detailLabel, &lv_font_montserrat_22,
                             LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(widgets.detailLabel, "");

  widgets.loadingLabel = lv_label_create(parent);
  lv_obj_set_pos(widgets.loadingLabel, 44, 332);
  lv_obj_set_size(widgets.loadingLabel, 430, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(widgets.loadingLabel, &lv_font_montserrat_18,
                             LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(widgets.loadingLabel, lv_color_hex(0xffff9800),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(widgets.loadingLabel, "");
  setHidden(widgets.loadingLabel, true);

  widgets.loadingBar = lv_bar_create(parent);
  lv_obj_set_pos(widgets.loadingBar, 44, 366);
  lv_obj_set_size(widgets.loadingBar, 430, 18);
  lv_bar_set_range(widgets.loadingBar, 0, 100);
  lv_bar_set_value(widgets.loadingBar, 0, LV_ANIM_OFF);
  lv_obj_set_style_bg_opa(widgets.loadingBar, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(widgets.loadingBar, lv_color_hex(0xffd0d0d0),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(widgets.loadingBar, lv_color_hex(0xffff9800),
                            LV_PART_INDICATOR | LV_STATE_DEFAULT);
  setHidden(widgets.loadingBar, true);
}

void updateMonitor(const app::ControllerStatus &status, size_t scheduleCount) {
  if (!widgets.processValue || !widgets.detailLabel ||
      !widgets.loadingLabel || !widgets.loadingBar) {
    return;
  }

  String processText;
  String detailText;

  if (status.state == app::ControllerState::Execution) {
    processText = app::reactionToDisplayName(status.execution.reaction);
    if (processText.length() == 0) {
      processText = "PROSES AKTIF";
    }
    detailText = buildExecutionDetailText(status.execution);
  }

  if (detailText.length() == 0 && status.waitingAck) {
    detailText = buildWaitingDetailText(status, scheduleCount);
  }

  lv_label_set_text(widgets.processValue, processText.c_str());
  lv_label_set_text(widgets.detailLabel, detailText.c_str());

  if (status.waitingAck) {
    lv_label_set_text(widgets.loadingLabel, buildWaitingStatusText(status).c_str());
    setHidden(widgets.loadingLabel, false);
    setHidden(widgets.loadingBar, false);
  } else {
    lv_label_set_text(widgets.loadingLabel, "");
    lv_bar_set_value(widgets.loadingBar, 0, LV_ANIM_OFF);
    loadingBarValue = 0;
    setHidden(widgets.loadingLabel, true);
    setHidden(widgets.loadingBar, true);
  }
}

void tickLoading(const app::ControllerStatus &status) {
  if (!widgets.loadingBar || !status.waitingAck) {
    return;
  }

  if (millis() - lastLoadingTickMs < kLoadingTickMs) {
    return;
  }

  lastLoadingTickMs = millis();
  loadingBarValue += 10;
  if (loadingBarValue > 100) {
    loadingBarValue = 0;
  }

  lv_bar_set_value(widgets.loadingBar, loadingBarValue, LV_ANIM_OFF);
}

}  // namespace runtime_ui
