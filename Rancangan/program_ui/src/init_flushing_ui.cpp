#include "init_flushing_ui.h"

#include <ArduinoJson.h>
#include <stdint.h>
#include <stdlib.h>

#include "controller_comm.h"
#include "ui/actions.h"
#include "ui/screens.h"

namespace {

// =============================================================
// BATAS PARAMETER INPUT — ubah nilai-nilai di sini untuk
// menyesuaikan batasan yang diizinkan oleh sistem.
// =============================================================

// Volume sampel / reaktor (mL)
constexpr long kVolumeMin                 = 50;
constexpr long kVolumeMax                 = 250;

// Volume H2SO4 (mL, float, opsional)
constexpr float kVolumeH2SO4Min           = 0.0f;
constexpr float kVolumeH2SO4Max           = 250.0f;

// Volume FeSO4 (mL, float, opsional)
constexpr float kVolumeFeSO4Min           = 0.0f;
constexpr float kVolumeFeSO4Max           = 250.0f;

// Total gabungan sampel + H2SO4 + FeSO4 tidak boleh melebihi
// kapasitas gelas beaker.
constexpr float kVolumeTotalMax           = 250.0f;

// =============================================================
constexpr size_t kScreenCount = 10;
constexpr uint32_t kLoadingTickMs = 100;

lv_obj_t *inisialisasiButtons[kScreenCount];
lv_obj_t *jadwalButtons[kScreenCount];
lv_obj_t *monitorButtons[kScreenCount];
lv_obj_t *spektroButtons[kScreenCount];
lv_obj_t *flushingButtons[kScreenCount];

lv_obj_t *inisialisasiWarningLabel = nullptr;
lv_obj_t *ackLoadingLabel = nullptr;
lv_obj_t *ackLoadingBar = nullptr;
lv_obj_t *flushAckLoadingLabel = nullptr;
lv_obj_t *flushAckLoadingBar = nullptr;

uint32_t lastLoadingTickMs = 0;
int loadingBarValue = 0;

struct SentInisialisasiConfig {
  float phValue = 0.0f;
  float feValue = 0.0f;
  int kalibrasi = 0;
  int prefilling = -1;
  bool valid = false;
};

SentInisialisasiConfig lastSentConfig;

struct StatusDisplay {
  const char *text;
  lv_color_t color;
};

const lv_color_t kColorRed = lv_color_hex(0xffdc1313);
const lv_color_t kColorGreen = lv_color_hex(0xff13dc6c);
const lv_color_t kColorBlack = lv_color_hex(0xff000000);
const lv_color_t kColorOrange = lv_color_hex(0xffff9800);

StatusDisplay phStatusDisplay(int status) {
  switch (status) {
    case -1: return {"Tidak diatur", kColorRed};
    case 0:  return {"Mengatur H2SO4...", kColorRed};
    case 1:  return {"H2SO4 sudah diatur", kColorGreen};
    default: return {"?", kColorBlack};
  }
}

StatusDisplay feStatusDisplay(int status) {
  switch (status) {
    case -1: return {"Tidak disesuaikan", kColorBlack};
    case 0:  return {"Menyesuaikan...", kColorBlack};
    case 1:  return {"Sudah disesuaikan", kColorGreen};
    default: return {"?", kColorBlack};
  }
}

const char *prefillStatusText(int status) {
  switch (status) {
    case -1: return "tidak prefilling";
    case 0:  return "prefilling....";
    case 1:  return "prefilling selesai!";
    default: return "";
  }
}

StatusDisplay kalibrasiStatusDisplay(int status) {
  switch (status) {
    case -1: return {"Tidak dikalibrasi", kColorBlack};
    case 0:  return {"Mengkalibrasi...", kColorBlack};
    case 1:  return {"Sudah dikalibrasi", kColorGreen};
    default: return {"?", kColorBlack};
  }
}

void applyStatusLabel(lv_obj_t *label, const StatusDisplay &display) {
  if (!label) {
    return;
  }

  lv_label_set_text(label, display.text);
  lv_obj_set_style_text_color(label, display.color,
                              LV_PART_MAIN | LV_STATE_DEFAULT);
}

int initialStatusFromConfig(float configValue) {
  return (configValue < 0.0f) ? -1 : 0;
}

int initialKalibrasiStatus(int configValue) {
  return (configValue == 1) ? 0 : -1;
}

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

void setDisabled(lv_obj_t *obj, bool disabled) {
  if (!obj) {
    return;
  }

  if (disabled) {
    lv_obj_add_state(obj, LV_STATE_DISABLED);
  } else {
    lv_obj_clear_state(obj, LV_STATE_DISABLED);
  }
}

void setArrayDisabled(lv_obj_t *buttons[], size_t count, bool disabled) {
  for (size_t i = 0; i < count; ++i) {
    setDisabled(buttons[i], disabled);
  }
}

void collectMenuButtons() {
  inisialisasiButtons[0] = objects.button_penjadwalan_proses_input_elektrofenton_21;
  inisialisasiButtons[1] = objects.button_penjadwalan_proses_input_elektrofenton_25;
  inisialisasiButtons[2] = objects.button_penjadwalan_proses_input_elektrofenton_29;
  inisialisasiButtons[3] = objects.button_penjadwalan_proses_input_elektrofenton_33;
  inisialisasiButtons[4] = objects.button_penjadwalan_proses_input_elektrofenton_37;
  inisialisasiButtons[5] = objects.button_penjadwalan_proses_input_elektrofenton_3;
  inisialisasiButtons[6] = objects.button_penjadwalan_proses_input_elektrofenton_5;
  inisialisasiButtons[7] = objects.button_penjadwalan_proses_input_elektrofenton_9;
  inisialisasiButtons[8] = objects.button_penjadwalan_proses_input_elektrofenton_13;
  inisialisasiButtons[9] = objects.button_penjadwalan_proses_input_elektrofenton_17;

  jadwalButtons[0] = objects.button_penjadwalan_proses_input_elektrofenton_22;
  jadwalButtons[1] = objects.button_penjadwalan_proses_input_elektrofenton_26;
  jadwalButtons[2] = objects.button_penjadwalan_proses_input_elektrofenton_30;
  jadwalButtons[3] = objects.button_penjadwalan_proses_input_elektrofenton_34;
  jadwalButtons[4] = objects.button_penjadwalan_proses_input_elektrofenton_38;
  jadwalButtons[5] = objects.button_penjadwalan_proses_input_elektrofenton_1;
  jadwalButtons[6] = objects.button_penjadwalan_proses_input_elektrofenton_6;
  jadwalButtons[7] = objects.button_penjadwalan_proses_input_elektrofenton_10;
  jadwalButtons[8] = objects.button_penjadwalan_proses_input_elektrofenton_14;
  jadwalButtons[9] = objects.button_penjadwalan_proses_input_elektrofenton_18;

  monitorButtons[0] = objects.button_monitor_proses_input_elektrofenton_6;
  monitorButtons[1] = objects.button_monitor_proses_input_elektrofenton_7;
  monitorButtons[2] = objects.button_monitor_proses_input_elektrofenton_8;
  monitorButtons[3] = objects.button_monitor_proses_input_elektrofenton_9;
  monitorButtons[4] = objects.button_monitor_proses_input_elektrofenton_10;
  monitorButtons[5] = objects.button_monitor_proses_input_elektrofenton_1;
  monitorButtons[6] = objects.button_monitor_proses_input_elektrofenton_2;
  monitorButtons[7] = objects.button_monitor_proses_input_elektrofenton_3;
  monitorButtons[8] = objects.button_monitor_proses_input_elektrofenton_4;
  monitorButtons[9] = objects.button_monitor_proses_input_elektrofenton_5;

  spektroButtons[0] = objects.button_penjadwalan_proses_input_elektrofenton_23;
  spektroButtons[1] = objects.button_penjadwalan_proses_input_elektrofenton_27;
  spektroButtons[2] = objects.button_penjadwalan_proses_input_elektrofenton_31;
  spektroButtons[3] = objects.button_penjadwalan_proses_input_elektrofenton_35;
  spektroButtons[4] = objects.button_penjadwalan_proses_input_elektrofenton_39;
  spektroButtons[5] = objects.button_penjadwalan_proses_input_elektrofenton_2;
  spektroButtons[6] = objects.button_penjadwalan_proses_input_elektrofenton_7;
  spektroButtons[7] = objects.button_penjadwalan_proses_input_elektrofenton_11;
  spektroButtons[8] = objects.button_penjadwalan_proses_input_elektrofenton_15;
  spektroButtons[9] = objects.button_penjadwalan_proses_input_elektrofenton_19;

  flushingButtons[0] = objects.button_penjadwalan_proses_input_elektrofenton_24;
  flushingButtons[1] = objects.button_penjadwalan_proses_input_elektrofenton_28;
  flushingButtons[2] = objects.button_penjadwalan_proses_input_elektrofenton_32;
  flushingButtons[3] = objects.button_penjadwalan_proses_input_elektrofenton_36;
  flushingButtons[4] = objects.button_penjadwalan_proses_input_elektrofenton_40;
  flushingButtons[5] = objects.button_penjadwalan_proses_input_elektrofenton_4;
  flushingButtons[6] = objects.button_penjadwalan_proses_input_elektrofenton_8;
  flushingButtons[7] = objects.button_penjadwalan_proses_input_elektrofenton_12;
  flushingButtons[8] = objects.button_penjadwalan_proses_input_elektrofenton_16;
  flushingButtons[9] = objects.button_penjadwalan_proses_input_elektrofenton_20;
}

const char *get_text_or_default(lv_obj_t *textarea, const char *fallback) {
  if (!textarea) {
    return fallback;
  }

  const char *text = lv_textarea_get_text(textarea);
  return (text && text[0] != '\0') ? text : fallback;
}

long parse_long(const char *text, long fallback) {
  if (!text || text[0] == '\0') {
    return fallback;
  }

  char *endPtr = nullptr;
  const long value = strtol(text, &endPtr, 10);
  return endPtr != text ? value : fallback;
}

bool try_parse_float(const char *text, float &value) {
  if (!text || text[0] == '\0') {
    return false;
  }

  char *endPtr = nullptr;
  value = strtof(text, &endPtr);
  if (endPtr == text) {
    return false;
  }

  while (*endPtr == ' ' || *endPtr == '\t') {
    ++endPtr;
  }

  return *endPtr == '\0';
}

bool textarea_has_value(lv_obj_t *textarea) {
  const char *text = get_text_or_default(textarea, "");
  return text && text[0] != '\0';
}

long read_long_textarea(lv_obj_t *textarea, long fallback) {
  return parse_long(get_text_or_default(textarea, ""), fallback);
}

const char *resolve_fe_type() {
  static char feType[4] = {0};
  if (!objects.fe_type_scrollbar) {
    return "mm";
  }

  feType[0] = '\0';
  lv_roller_get_selected_str(objects.fe_type_scrollbar, feType, sizeof(feType));
  if (feType[0] != '\0') {
    return feType;
  }

  const uint16_t selected = lv_roller_get_selected(objects.fe_type_scrollbar);
  return (selected == 1) ? "ml" : "mm";
}

void appendWarning(String &msg, const char *line) {
  if (msg.length() == 0) {
    msg = "PERINGATAN:\n";
  } else {
    msg += "\n";
  }

  msg += "- ";
  msg += line;
}

void showWarningBox(const String &message) {
  if (!objects.messagebox_peringatan_input_inisialisasi || !inisialisasiWarningLabel) {
    return;
  }

  lv_label_set_text(inisialisasiWarningLabel, message.c_str());
  setHidden(objects.messagebox_peringatan_input_inisialisasi, false);
  lv_obj_move_foreground(objects.messagebox_peringatan_input_inisialisasi);
}

void updateFlushingProgressText(int percent) {
  lv_obj_t *container = objects.container_progress_flushing;
  if (!container || lv_obj_get_child_cnt(container) == 0) {
    return;
  }

  lv_obj_t *label = lv_obj_get_child(container, 0);
  if (!label) {
    return;
  }

  char buf[64];
  snprintf(buf, sizeof(buf), "Progress Flushing (%d%%)", percent);
  lv_label_set_text(label, buf);
}

void applyInitStatusLabels(int h2so4, int feso4, int kalibrasi) {
  applyStatusLabel(objects.teks_status_mengatur_ph, phStatusDisplay(h2so4));
  applyStatusLabel(objects.teks_status_konsentrasi_fe2, feStatusDisplay(feso4));
  applyStatusLabel(objects.teks_status_kalibrasi_spektro, kalibrasiStatusDisplay(kalibrasi));
}

void applyInitProgressBar(int sampelPercent) {
  if (objects.teks_progress_mengisi_reaktor) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Mengisi Reaktor (%d%%)", sampelPercent);
    lv_label_set_text(objects.teks_progress_mengisi_reaktor, buf);
  }

  if (objects.progress_bar_mengisi_reaktor) {
    lv_bar_set_value(objects.progress_bar_mengisi_reaktor,
                     sampelPercent, LV_ANIM_OFF);
  }
}

}  // namespace

namespace init_flushing_ui {

void begin() {
  collectMenuButtons();

  if (objects.messagebox_peringatan_input_inisialisasi) {
    lv_obj_set_width(objects.messagebox_peringatan_input_inisialisasi, 360);
    lv_obj_set_height(objects.messagebox_peringatan_input_inisialisasi, LV_SIZE_CONTENT);

    lv_obj_t *content =
        lv_msgbox_get_content(objects.messagebox_peringatan_input_inisialisasi);
    if (content) {
      lv_obj_set_style_pad_left(content, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_top(content, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_right(content, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_pad_bottom(content, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

      if (objects.pesan_peringatan_2) {
        lv_obj_add_flag(objects.pesan_peringatan_2, LV_OBJ_FLAG_HIDDEN);
      }

      if (!inisialisasiWarningLabel) {
        inisialisasiWarningLabel = lv_label_create(content);
        lv_obj_set_width(inisialisasiWarningLabel, LV_PCT(100));
        lv_label_set_long_mode(inisialisasiWarningLabel, LV_LABEL_LONG_WRAP);
        lv_obj_set_style_text_color(inisialisasiWarningLabel,
                                    lv_color_hex(0xff5f2120),
                                    LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(inisialisasiWarningLabel, "");
      }
    }

    lv_obj_t *closeButton =
        lv_msgbox_get_close_btn(objects.messagebox_peringatan_input_inisialisasi);
    if (closeButton) {
      lv_obj_remove_event_cb(closeButton, NULL);
      lv_obj_add_event_cb(closeButton,
                          action_tutup_pesan_error_input_inisialisasi,
                          LV_EVENT_CLICKED, nullptr);
    }

    setHidden(objects.messagebox_peringatan_input_inisialisasi, true);
  }

  if (objects.container_content_6) {
    ackLoadingLabel = lv_label_create(objects.container_content_6);
    lv_obj_set_pos(ackLoadingLabel, 26, 340);
    lv_obj_set_size(ackLoadingLabel, 430, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(ackLoadingLabel, &lv_font_montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ackLoadingLabel, kColorOrange,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(ackLoadingLabel, "");
    setHidden(ackLoadingLabel, true);

    ackLoadingBar = lv_bar_create(objects.container_content_6);
    lv_obj_set_pos(ackLoadingBar, 26, 370);
    lv_obj_set_size(ackLoadingBar, 430, 18);
    lv_bar_set_range(ackLoadingBar, 0, 100);
    lv_bar_set_value(ackLoadingBar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_opa(ackLoadingBar, LV_OPA_40,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ackLoadingBar, lv_color_hex(0xffd0d0d0),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ackLoadingBar, kColorOrange,
                              LV_PART_INDICATOR | LV_STATE_DEFAULT);
    setHidden(ackLoadingBar, true);
  }

  if (objects.container_content_9) {
    flushAckLoadingLabel = lv_label_create(objects.container_content_9);
    lv_obj_set_pos(flushAckLoadingLabel, 14, 314);
    lv_obj_set_size(flushAckLoadingLabel, 662, LV_SIZE_CONTENT);
    lv_obj_set_style_text_font(flushAckLoadingLabel, &lv_font_montserrat_18,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(flushAckLoadingLabel, kColorOrange,
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(flushAckLoadingLabel, "");
    setHidden(flushAckLoadingLabel, true);

    flushAckLoadingBar = lv_bar_create(objects.container_content_9);
    lv_obj_set_pos(flushAckLoadingBar, 14, 344);
    lv_obj_set_size(flushAckLoadingBar, 662, 18);
    lv_bar_set_range(flushAckLoadingBar, 0, 100);
    lv_bar_set_value(flushAckLoadingBar, 0, LV_ANIM_OFF);
    lv_obj_set_style_bg_opa(flushAckLoadingBar, LV_OPA_40,
                            LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(flushAckLoadingBar, lv_color_hex(0xffd0d0d0),
                              LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(flushAckLoadingBar, kColorOrange,
                              LV_PART_INDICATOR | LV_STATE_DEFAULT);
    setHidden(flushAckLoadingBar, true);
  }

  setHidden(objects.messagebox_konfirmasi_flushing, true);
  setHidden(objects.container_progress_flushing, true);
  setHidden(objects.message_flushing_sukses, true);
}

void syncMenuButtons(const app::ControllerStatus &status) {
  bool initEnabled = true;
  bool jadwalEnabled = true;
  bool monitorEnabled = true;
  bool spektroEnabled = true;
  bool flushingEnabled = true;
  bool forceStopVisible = false;

  switch (status.state) {
    case app::ControllerState::InisialisasiInput:
    case app::ControllerState::InisialisasiEksekusi:
      jadwalEnabled = false;
      monitorEnabled = false;
      spektroEnabled = false;
      flushingEnabled = false;
      break;

    case app::ControllerState::Idle:
      break;

    case app::ControllerState::Execution:
      forceStopVisible = true;
      break;

    case app::ControllerState::Flushing:
      initEnabled = false;
      jadwalEnabled = false;
      monitorEnabled = false;
      spektroEnabled = false;
      break;
  }

  if (status.waitingAck) {
    if (status.pendingType == app::PendingCommandType::StartInisialisasi) {
      jadwalEnabled = false;
      monitorEnabled = false;
      spektroEnabled = false;
      flushingEnabled = false;
    } else if (status.pendingType == app::PendingCommandType::StartFlushing) {
      initEnabled = false;
      jadwalEnabled = false;
      monitorEnabled = false;
      spektroEnabled = false;
    }
  }

  setArrayDisabled(inisialisasiButtons, kScreenCount, !initEnabled);
  setArrayDisabled(jadwalButtons, kScreenCount, !jadwalEnabled);
  setArrayDisabled(monitorButtons, kScreenCount, !monitorEnabled);
  setArrayDisabled(spektroButtons, kScreenCount, !spektroEnabled);
  setArrayDisabled(flushingButtons, kScreenCount, !flushingEnabled);
  setHidden(objects.button_force_stop, !forceStopVisible);
}

void syncInisialisasiScreen(const app::ControllerStatus &status) {
  const app::InisialisasiStatus &init = status.inisialisasi;
  bool isWaitingInitAck =
      status.waitingAck &&
      status.pendingType == app::PendingCommandType::StartInisialisasi;

  setHidden(objects.pindah_ke_jadwal, true);

  if (isWaitingInitAck) {
    if (objects.status_inisialisasi) {
      String text = "Menunggu ";
      text += status.expectedAck ? status.expectedAck : "ACK";
      text += " | kirim ke-";
      text += String(status.sendCount);
      lv_label_set_text(objects.status_inisialisasi, text.c_str());
    }

    setHidden(ackLoadingLabel, false);
    setHidden(ackLoadingBar, false);
    if (ackLoadingLabel) {
      String loadText = "Mengirim konfigurasi ke controller...";
      lv_label_set_text(ackLoadingLabel, loadText.c_str());
    }

    applyInitProgressBar(0);

    if (lastSentConfig.valid) {
      int h2so4Init = initialStatusFromConfig(lastSentConfig.phValue);
      int feso4Init = initialStatusFromConfig(lastSentConfig.feValue);
      int kalInit = initialKalibrasiStatus(lastSentConfig.kalibrasi);
      applyInitStatusLabels(h2so4Init, feso4Init, kalInit);
    }
    if (objects.teks_status_prefilling) {
      int prefillInit = (lastSentConfig.prefilling == 1) ? 0 : -1;
      lv_label_set_text(objects.teks_status_prefilling,
                        prefillStatusText(prefillInit));
    }
    return;
  }

  setHidden(ackLoadingLabel, true);
  setHidden(ackLoadingBar, true);

  if (status.state == app::ControllerState::InisialisasiEksekusi) {
    applyInitProgressBar(init.sampelPercent);
    applyInitStatusLabels(init.h2so4Status, init.feso4Status, init.kalibrasiStatus);
    if (objects.status_inisialisasi) {
      lv_label_set_text(objects.status_inisialisasi,
                        init.done ? "Inisialisasi Selesai" : "Inisialisasi Berlangsung...");
    }
    if (objects.teks_status_prefilling) {
      lv_label_set_text(objects.teks_status_prefilling,
                        prefillStatusText(init.prefillStatus));
    }
    setHidden(objects.pindah_ke_jadwal, !init.done);
    return;
  }

  if (status.state == app::ControllerState::Idle && init.done) {
    applyInitProgressBar(init.sampelPercent);
    applyInitStatusLabels(init.h2so4Status, init.feso4Status, init.kalibrasiStatus);
    if (objects.status_inisialisasi) {
      lv_label_set_text(objects.status_inisialisasi, "Inisialisasi Selesai");
    }
    if (objects.teks_status_prefilling) {
      lv_label_set_text(objects.teks_status_prefilling,
                        prefillStatusText(init.prefillStatus));
    }
    setHidden(objects.pindah_ke_jadwal, false);
  }
}

void syncFlushingScreen(const app::ControllerStatus &status) {
  const app::FlushingStatus &flush = status.flushing;
  bool isFlushing = status.state == app::ControllerState::Flushing;
  bool isWaitingFlushAck =
      status.waitingAck &&
      status.pendingType == app::PendingCommandType::StartFlushing;

  if (flush.done) {
    setHidden(objects.button_start_jadwal_2, true);
    setHidden(objects.container_progress_flushing, false);
    setHidden(flushAckLoadingLabel, true);
    setHidden(flushAckLoadingBar, true);
    if (flushAckLoadingBar) {
      lv_bar_set_value(flushAckLoadingBar, 0, LV_ANIM_OFF);
    }
    updateFlushingProgressText(flush.progress);

    if (objects.progress_bar_proses_flushing) {
      lv_bar_set_value(objects.progress_bar_proses_flushing,
                       flush.progress, LV_ANIM_OFF);
    }

    setHidden(objects.message_flushing_sukses, false);
    if (objects.message_flushing_sukses) {
      lv_obj_move_foreground(objects.message_flushing_sukses);
    }
    return;
  }

  if (isFlushing) {
    setHidden(objects.button_start_jadwal_2, true);
    setHidden(objects.container_progress_flushing, false);
    setHidden(flushAckLoadingLabel, true);
    setHidden(flushAckLoadingBar, true);
    if (flushAckLoadingBar) {
      lv_bar_set_value(flushAckLoadingBar, 0, LV_ANIM_OFF);
    }
    setHidden(objects.message_flushing_sukses, true);
    updateFlushingProgressText(flush.progress);

    if (objects.progress_bar_proses_flushing) {
      lv_bar_set_value(objects.progress_bar_proses_flushing,
                       flush.progress, LV_ANIM_OFF);
    }
    return;
  }

  if (isWaitingFlushAck) {
    setHidden(objects.button_start_jadwal_2, true);
    setHidden(objects.container_progress_flushing, true);
    setHidden(flushAckLoadingLabel, false);
    setHidden(flushAckLoadingBar, false);
    setHidden(objects.message_flushing_sukses, true);

    if (flushAckLoadingLabel) {
      String text = "Menunggu ";
      text += status.expectedAck ? status.expectedAck : "ACK_FLUSHING";
      text += " | kirim ke-";
      text += String(status.sendCount);
      lv_label_set_text(flushAckLoadingLabel, text.c_str());
    }
    return;
  }

  setHidden(objects.button_start_jadwal_2, false);
  setHidden(objects.container_progress_flushing, true);
  setHidden(flushAckLoadingLabel, true);
  setHidden(flushAckLoadingBar, true);
  if (flushAckLoadingBar) {
    lv_bar_set_value(flushAckLoadingBar, 0, LV_ANIM_OFF);
  }
  setHidden(objects.message_flushing_sukses, true);
}

void tickLoading(const app::ControllerStatus &status) {
  if (!status.waitingAck) {
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

  if (status.pendingType == app::PendingCommandType::StartInisialisasi) {
    if (ackLoadingBar) {
      lv_bar_set_value(ackLoadingBar, loadingBarValue, LV_ANIM_OFF);
    }
  }

  if (status.pendingType == app::PendingCommandType::StartFlushing) {
    if (flushAckLoadingBar) {
      lv_bar_set_value(flushAckLoadingBar, loadingBarValue, LV_ANIM_OFF);
    }
  }
}

bool validateAndStartInisialisasi() {
  long volume = read_long_textarea(objects.textarea_input_volume, 0);
  const char *h2so4Text = get_text_or_default(objects.textarea_input_ph, "");
  bool h2so4Provided = h2so4Text && h2so4Text[0] != '\0';
  float h2so4Value = -1.0f;
  const char *feText = get_text_or_default(objects.textarea_input_arus_1, "");
  bool feProvided = feText && feText[0] != '\0';
  float feValue = -1.0f;

  String warning;

  if (volume < kVolumeMin || volume > kVolumeMax) {
    appendWarning(warning, "Volume sampel harus 50-250 mL.");
  }

  if (h2so4Provided && !try_parse_float(h2so4Text, h2so4Value)) {
    appendWarning(warning, "Volume H2SO4 harus angka.");
  } else if (h2so4Provided && (h2so4Value < kVolumeH2SO4Min || h2so4Value > kVolumeH2SO4Max)) {
    char buf[80];
    snprintf(buf, sizeof(buf),
             "Volume H2SO4 harus dalam rentang %.0f-%.0f mL.",
             kVolumeH2SO4Min, kVolumeH2SO4Max);
    appendWarning(warning, buf);
  }

  if (feProvided && !try_parse_float(feText, feValue)) {
    appendWarning(warning, "Jumlah Fe2+ harus angka.");
  } else if (feProvided && (feValue < kVolumeFeSO4Min || feValue > kVolumeFeSO4Max)) {
    char buf[80];
    snprintf(buf, sizeof(buf),
             "Volume FeSO4 harus dalam rentang %.0f-%.0f mL.",
             kVolumeFeSO4Min, kVolumeFeSO4Max);
    appendWarning(warning, buf);
  }

  // Cek total: hanya hitung nilai yang berhasil diparsing.
  // Jika field opsional kosong, kontribusinya 0.
  {
    float h2so4ForTotal = (h2so4Provided && h2so4Value >= 0.0f) ? h2so4Value : 0.0f;
    float feForTotal    = (feProvided    && feValue    >= 0.0f) ? feValue    : 0.0f;
    float totalVolume   = (float)volume + h2so4ForTotal + feForTotal;

    if (totalVolume > kVolumeTotalMax) {
      char buf[120];
      snprintf(buf, sizeof(buf),
               "Total volume (sampel + H2SO4 + FeSO4) melebihi %.0f mL "
               "(total saat ini: %.1f mL).",
               kVolumeTotalMax, totalVolume);
      appendWarning(warning, buf);
    }
  }

  if (warning.length() > 0) {
    showWarningBox(warning);
    return false;
  }

  setHidden(objects.messagebox_peringatan_input_inisialisasi, true);

  bool prefillOn = objects.switch_prefill_pompa &&
                   lv_obj_has_state(objects.switch_prefill_pompa, LV_STATE_CHECKED);

  float phValue = h2so4Provided ? h2so4Value : -1.0f;
  float feValueFinal = feProvided ? feValue : -1.0f;
  int kalInt = 1;
  int prefillInt = prefillOn ? 1 : -1;

  lastSentConfig.phValue = phValue;
  lastSentConfig.feValue = feValueFinal;
  lastSentConfig.kalibrasi = kalInt;
  lastSentConfig.prefilling = prefillInt;
  lastSentConfig.valid = true;

  JsonDocument doc;
  doc["status"] = "inisialisasi";
  doc["vol"] = volume;
  doc["ph"] = phValue;
  doc["fe_type"] = resolve_fe_type();
  doc["fe"] = feValueFinal;
  doc["kalibrasi"] = kalInt;
  doc["prefilling"] = prefillInt;

  String payload;
  serializeJson(doc, payload);

  return controller_comm::startInisialisasi(payload);
}

bool requestFlushing() {
  return controller_comm::startFlushing();
}

}  // namespace init_flushing_ui
