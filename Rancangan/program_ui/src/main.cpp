#include <Arduino.h>
#include <esp_system.h>
#include <PCA9557.h>
#include <lvgl.h>
#include <Crowbits_DHT20.h>

#include <SPI.h>
#include <Adafruit_GFX.h>

#include "ui/ui.h"
#include "ui/vars.h"
#include "ui/actions.h"
#include "lgfx/lgfx.h"
#include "controller_comm.h"
#include "runtime_ui.h"
#include "schedule_ui.h"
#include "init_flushing_ui.h"
#include "spektro_ui.h"
#include "sd_storage.h"
#include "keypad.h"

namespace {

constexpr uint32_t kDebugBaudRate = 115200;
constexpr uint32_t kControllerBaudRate = 115200;
constexpr int kControllerRxPin = 44;
constexpr int kControllerTxPin = 43;
constexpr uint32_t kAckRetryIntervalMs = 1500;
constexpr uint32_t kRxIdleFlushMs = 25;

HardwareSerial controllerUart(1);

controller_comm::Config make_controller_config() {
  controller_comm::Config config;
  config.baudRate = kControllerBaudRate;
  config.rxPin = kControllerRxPin;
  config.txPin = kControllerTxPin;
  config.retryIntervalMs = kAckRetryIntervalMs;
  config.rxIdleFlushMs = kRxIdleFlushMs;
  return config;
}

controller_comm::Config controllerConfig = make_controller_config();

lv_obj_t *activeKeyboard = nullptr;
lv_obj_t *activeTextarea = nullptr;

bool gSdSaveRequested = false;
bool gSdSaveInProgress = false;

// Breadcrumb di RTC memory: survive software reset / panic / watchdog /
// brownout (hilang hanya saat power-cycle penuh). Dipakai untuk tahu apakah
// reset terjadi tepat saat menulis SD.
constexpr uint32_t kBreadcrumbMagic = 0x5D10C0DE;
enum SavePhase : uint32_t {
  kPhaseIdle       = 0,
  kPhaseBeforeSave = 1,
  kPhaseAfterSave  = 2,
};
RTC_NOINIT_ATTR uint32_t gBreadcrumbMagic;
RTC_NOINIT_ATTR uint32_t gBreadcrumbPhase;

// Langkah loop terakhir sebelum hang. Saat reset watchdog (reason 7 = hard
// lockup), nilai ini menunjuk persis di mana CPU nyangkut.
RTC_NOINIT_ATTR uint32_t gLoopStep;

void setBreadcrumb(uint32_t phase) {
  gBreadcrumbMagic = kBreadcrumbMagic;
  gBreadcrumbPhase = phase;
}

inline void markStep(uint32_t step) { gLoopStep = step; }

bool is_elektrofenton_textarea(lv_obj_t *textarea) {
  return textarea == objects.textarea_input_magnetic_stirrer_speed ||
         textarea == objects.textarea_input_suhu ||
         textarea == objects.textarea_input_arus ||
         textarea == objects.textarea_input_jam ||
         textarea == objects.textarea_input_menit;
}

bool is_adsorpsi_textarea(lv_obj_t *textarea) {
  return textarea == objects.textarea_input_magnetic_stirrer_speed_1 ||
         textarea == objects.textarea_input_suhu_1 ||
         textarea == objects.textarea_input_jam_1 ||
         textarea == objects.textarea_input_menit_1;
}

bool is_inisialisasi_textarea(lv_obj_t *textarea) {
  return textarea == objects.textarea_input_volume ||
         textarea == objects.textarea_input_ph ||
         textarea == objects.textarea_input_arus_1;
}

lv_obj_t *get_keyboard_for_textarea(lv_obj_t *textarea) {
  if (is_elektrofenton_textarea(textarea)) {
    return objects.keyboard_elektrofenton;
  }

  if (is_adsorpsi_textarea(textarea)) {
    return objects.keyboard_adsorpsi;
  }

  if (is_inisialisasi_textarea(textarea)) {
    return objects.keyboard_elektrofenton_1;
  }

  return nullptr;
}

void hide_keyboard(lv_obj_t *keyboard) {
  if (!keyboard) {
    return;
  }

  lv_keyboard_set_textarea(keyboard, nullptr);
  lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);

  if (activeKeyboard == keyboard) {
    activeKeyboard = nullptr;
  }
}

void hide_all_keyboards() {
  if (activeTextarea) {
    lv_obj_clear_state(activeTextarea, LV_STATE_FOCUSED);
    activeTextarea = nullptr;
  }

  hide_keyboard(objects.keyboard_elektrofenton);
  hide_keyboard(objects.keyboard_adsorpsi);
  hide_keyboard(objects.keyboard_elektrofenton_1);
}

void show_keyboard_for_textarea(lv_obj_t *textarea) {
  lv_obj_t *keyboard = get_keyboard_for_textarea(textarea);
  if (!keyboard) {
    return;
  }

  if (activeKeyboard && activeKeyboard != keyboard) {
    hide_keyboard(activeKeyboard);
  }

  lv_keyboard_set_textarea(keyboard, textarea);
  lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(keyboard);

  activeKeyboard = keyboard;
  activeTextarea = textarea;
}

void set_hidden(lv_obj_t *obj, bool hidden) {
  if (!obj) {
    return;
  }

  if (hidden) {
    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
  } else {
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  }
}

void hide_transient_ui() {
  schedule_ui::hideWarningBoxes();
  set_hidden(objects.messagebox_peringatan_input_inisialisasi, true);
  set_hidden(objects.messagebox_konfirmasi_flushing, true);
  set_hidden(objects.message_flushing_sukses, true);
}

void show_overlay(lv_obj_t *obj) {
  if (!obj) {
    return;
  }

  lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(obj);
}

void refresh_all_ui() {
  const app::ControllerStatus &status = controller_comm::status();
  schedule_ui::sync(status);
  runtime_ui::updateMonitor(status, schedule_ui::count());
  init_flushing_ui::syncMenuButtons(status);
  init_flushing_ui::syncInisialisasiScreen(status);
  init_flushing_ui::syncFlushingScreen(status);
}

void navigate_to(enum ScreensEnum screenId) {
  hide_all_keyboards();
  hide_transient_ui();
  loadScreen(screenId);
  refresh_all_ui();
}

void intro_timer_cb(lv_timer_t *t) {
  lv_timer_del(t);
  navigate_to(SCREEN_ID_MENU_INISIALISASI);
}

bool is_idle_or_execution() {
  const auto &s = controller_comm::status();
  return s.state == app::ControllerState::Idle ||
         s.state == app::ControllerState::Execution;
}

// ---- Storage status messagebox helpers ----

constexpr uint32_t kStatusBoxBgRed   = 0xfff7b6b6;  // soft red
constexpr uint32_t kStatusBoxBgGreen = 0xffb6f7c4;  // soft green

void pick_status_messagebox(lv_obj_t *&outBox, lv_obj_t *&outLabel) {
  lv_obj_t *active = lv_scr_act();
  if (active == objects.menu_spektro_degradasi_warna) {
    outBox   = objects.messagebox_status_penyimpanan_2;
    outLabel = objects.pesan_peringatan_6;
    return;
  }
  outBox   = objects.messagebox_status_penyimpanan;
  outLabel = objects.pesan_peringatan_5;
}

void show_status_box(lv_obj_t *box, lv_obj_t *label,
                     uint32_t bgColorHex, const String &text) {
  if (!box) return;
  lv_obj_set_style_bg_color(box, lv_color_hex(bgColorHex),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  if (label) {
    lv_label_set_text(label, text.c_str());
    lv_obj_set_style_text_color(label, lv_color_hex(0xff111111),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  }
  lv_obj_clear_flag(box, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(box);
}

void hide_all_status_messageboxes() {
  set_hidden(objects.messagebox_status_penyimpanan, true);
  set_hidden(objects.messagebox_status_penyimpanan_2, true);
}

// Tombol-X (close btn) bawaan lv_msgbox memanggil lv_obj_del() -> MENGHAPUS
// objek msgbox-nya. Padahal seluruh kode me-reuse objek msgbox (show/hide)
// berulang kali. Sekali user klik X, pointer objects.* jadi dangling dan akses
// berikutnya = use-after-free -> panic/hang acak saat render (terlihat pada
// flow simpan SD: reset_reason 4/7 di lv_timer_handler).
//
// Solusi mengikuti pola yang sudah dipakai untuk messagebox_peringatan_input_
// inisialisasi (lihat init_flushing_ui::begin): ganti perilaku tombol-X dari
// delete menjadi hide.
void msgbox_close_to_hide_cb(lv_event_t *e) {
  lv_obj_t *btn = lv_event_get_target(e);
  lv_obj_t *mbox = btn ? lv_obj_get_parent(btn) : nullptr;
  if (mbox) lv_obj_add_flag(mbox, LV_OBJ_FLAG_HIDDEN);
}

void harden_status_messageboxes() {
  lv_obj_t *boxes[] = {
      objects.messagebox_status_penyimpanan,
      objects.messagebox_status_penyimpanan_2,
      objects.messagebox_peringatan_elektrofenton,
      objects.messagebox_peringatan_adsorpsi,
      objects.messagebox_konfirmasi_flushing,
      objects.message_flushing_sukses,
  };
  for (lv_obj_t *box : boxes) {
    if (!box) continue;
    lv_obj_t *closeBtn = lv_msgbox_get_close_btn(box);
    if (!closeBtn) continue;
    lv_obj_remove_event_cb(closeBtn, NULL);  // buang cb delete bawaan
    lv_obj_add_event_cb(closeBtn, msgbox_close_to_hide_cb, LV_EVENT_CLICKED,
                        nullptr);
  }
}

void process_sd_save_request() {
  if (!gSdSaveRequested || gSdSaveInProgress) {
    return;
  }

  gSdSaveRequested = false;
  gSdSaveInProgress = true;

  setBreadcrumb(kPhaseBeforeSave);
  markStep(160);
  const sd_storage::SaveResult result = sd_storage::saveAllSpektroDanDegradasi();
  setBreadcrumb(kPhaseAfterSave);
  markStep(161);

  gSdSaveInProgress = false;

  lv_obj_t *box = nullptr;
  lv_obj_t *label = nullptr;
  pick_status_messagebox(box, label);
  markStep(162);

  switch (result.status) {
    case sd_storage::SaveStatus::Ok: {
      show_status_box(box, label, kStatusBoxBgGreen,
                      String("Semua Data Berhasil Disimpan"));
      break;
    }
    case sd_storage::SaveStatus::WriteError: {
      String text = "ERROR: gagal menulis";
      if (result.fileCount > 0) {
        text += "\nTersimpan sebagian";
      }
      if (result.errorDetail.length() > 0) {
        text += "\n";
        text += result.errorDetail;
      }
      show_status_box(box, label, kStatusBoxBgRed, text);
      break;
    }
    case sd_storage::SaveStatus::Empty: {
      show_status_box(box, label, kStatusBoxBgRed,
                      String("Tidak ada data untuk disimpan"));
      break;
    }
    case sd_storage::SaveStatus::NoCard:
    default: {
      show_status_box(box, label, kStatusBoxBgRed,
                      String("SD CARD BELUM DIMOUNT"));
      break;
    }
  }
}

}  // namespace

extern "C" {

void action_tampil_numpad_rpm(lv_event_t *e) {
  show_keyboard_for_textarea(lv_event_get_target(e));
}

void action_tampil_numpad_suhu(lv_event_t *e) {
  show_keyboard_for_textarea(lv_event_get_target(e));
}

void action_tampil_numpad_arus_listrik(lv_event_t *e) {
  show_keyboard_for_textarea(lv_event_get_target(e));
}

void action_tampil_numpad_jam(lv_event_t *e) {
  show_keyboard_for_textarea(lv_event_get_target(e));
}

void action_tampil_numpad_menit(lv_event_t *e) {
  show_keyboard_for_textarea(lv_event_get_target(e));
}

void action_sembunyikan_numpad(lv_event_t *e) {
  hide_keyboard(lv_event_get_target(e));

  if (activeTextarea) {
    lv_obj_clear_state(activeTextarea, LV_STATE_FOCUSED);
    activeTextarea = nullptr;
  }
}

void action_next_page(lv_event_t *e) {
  (void)e;
  schedule_ui::nextPage();
}

void action_back_page(lv_event_t *e) {
  (void)e;
  schedule_ui::backPage();
}

void action_pindah_ke_menu_penjadwalan_proses_utama(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_UTAMA);
}

void action_pindah_ke_menu_monitor(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_MONITOR);
}

void action_pindah_ke_menu_penjadwalan_proses_pilih(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_PILIH);
}

void action_start_dan_kirim_jadwal(lv_event_t *e) {
  (void)e;
  if (!schedule_ui::startSend()) {
    return;
  }

  navigate_to(SCREEN_ID_MENU_MONITOR);
}

void action_pindah_ke_menu_penjadwalan_proses_input_elektrofenton(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_INPUT_ELEKTROFENTON);
}

void action_pindah_ke_menu_penjadwalan_proses_input_adsorpsi(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_INPUT_ADSORPSI);
}

void action_kembali_ke_menu_penjadwalan_proses_pilih(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_PILIH);
}

void action_tambahkan_jadwal_elektrofenton(lv_event_t *e) {
  (void)e;
  hide_all_keyboards();
  if (schedule_ui::addElectroFenton()) {
    navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_UTAMA);
  }
}

void action_tambahkan_jadwal_adsorpsi(lv_event_t *e) {
  (void)e;
  hide_all_keyboards();
  if (schedule_ui::addAdsorpsi()) {
    navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_UTAMA);
  }
}

void action_lakukan_force_stop(lv_event_t *e) {
  (void)e;
  if (controller_comm::status().state != app::ControllerState::Execution) return;
  controller_comm::requestForceStop();
  navigate_to(SCREEN_ID_MENU_MONITOR);
}

void action_pindah_ke_menu_spektro(lv_event_t *e) {
  (void)e;
  if (!is_idle_or_execution()) return;
  navigate_to(SCREEN_ID_MENU_SPEKTRO_DEGRADASI_WARNA);
  spektro_ui::onSpektroScreenActivated();
}

void action_pindah_ke_menu_spektro_spektrum_warna(lv_event_t *e) {
  (void)e;
  navigate_to(SCREEN_ID_MENU_SPEKTRO_SPEKTRUM_WARNA);
  spektro_ui::onSpektroScreenActivated();
}

void action_ke_menu_degradasi_warna(lv_event_t *e) {
  (void)e;
  navigate_to(SCREEN_ID_MENU_SPEKTRO_DEGRADASI_WARNA);
  spektro_ui::onSpektroScreenActivated();
}

void action_tambahkan_spektrofotometri_dan_kembali_ke_menu_penjadwalan_proses(lv_event_t *e) {
  (void)e;
  if (!schedule_ui::addSpektrofotometri()) return;
  navigate_to(SCREEN_ID_MENU_PENJADWALAN_PROSES_UTAMA);
}

void action_pindah_ke_menu_flushing(lv_event_t *e) {
  (void)e;
  const auto &s = controller_comm::status();
  if (s.state != app::ControllerState::Idle &&
      s.state != app::ControllerState::Execution &&
      s.state != app::ControllerState::Flushing) {
    return;
  }
  navigate_to(SCREEN_ID_MENU_FLUSHING);
}

void action_pindah_ke_menu_inisialisasi(lv_event_t *e) {
  (void)e;
  const auto &s = controller_comm::status();
  if (s.state == app::ControllerState::Flushing) return;

  if (s.state == app::ControllerState::InisialisasiInput) {
    navigate_to(SCREEN_ID_MENU_INISIALISASI);
  } else {
    navigate_to(SCREEN_ID_MENU_MELAKUKAN_INISIALISASI);
  }
}

void action_mulai_inisialisasi(lv_event_t *e) {
  (void)e;
  hide_all_keyboards();

  if (!init_flushing_ui::validateAndStartInisialisasi()) {
    return;
  }

  navigate_to(SCREEN_ID_MENU_MELAKUKAN_INISIALISASI);
}

void action_tutup_pesan_error_input_inisialisasi(lv_event_t *e) {
  (void)e;
  set_hidden(objects.messagebox_peringatan_input_inisialisasi, true);
}

void action_tampilkan_message_konfirmasi_flushing(lv_event_t *e) {
  (void)e;
  if (controller_comm::status().state != app::ControllerState::Idle) return;
  show_overlay(objects.messagebox_konfirmasi_flushing);
}

void action_lakukan_flushing(lv_event_t *e) {
  (void)e;
  set_hidden(objects.messagebox_konfirmasi_flushing, true);

  if (!init_flushing_ui::requestFlushing()) {
    return;
  }

  refresh_all_ui();
}

void action_simpan_data_spektro_ke_sdcard(lv_event_t *e) {
  (void)e;

  lv_obj_t *box = nullptr;
  lv_obj_t *label = nullptr;
  pick_status_messagebox(box, label);

  if (gSdSaveInProgress) {
    show_status_box(box, label, kStatusBoxBgGreen,
                    String("Sedang menyimpan..."));
    return;
  }

  // Probe / try mount before writing.
  if (!sd_storage::isMounted()) {
    if (!sd_storage::tryRemount()) {
      show_status_box(box, label, kStatusBoxBgRed,
                      String("SD CARD BELUM DIMOUNT"));
      return;
    }
  }

  if (gSdSaveRequested) {
    show_status_box(box, label, kStatusBoxBgGreen,
                    String("Sedang menyimpan..."));
    return;
  }

  gSdSaveRequested = true;
  show_status_box(box, label, kStatusBoxBgGreen,
                  String("Sedang menyimpan..."));
}

void action_tutup_pesan_status_penyimpanan_data_spektro(lv_event_t *e) {
  (void)e;
  hide_all_status_messageboxes();
}

}  // extern "C"

void setup() {
  delay(1000);
  Serial.begin(kDebugBaudRate);
  delay(2000);

  const int resetReason = static_cast<int>(esp_reset_reason());
  Serial.printf("[boot] reset reason: %d\n", resetReason);

  // Snapshot breadcrumb sebelum di-reset; valid hanya jika magic cocok
  // (kalau cold boot / power-cycle isinya sampah).
  const bool breadcrumbValid = (gBreadcrumbMagic == kBreadcrumbMagic);
  const uint32_t lastPhase = breadcrumbValid ? gBreadcrumbPhase : 0xFFFFFFFFu;
  const uint32_t lastStep  = breadcrumbValid ? gLoopStep : 0xFFFFFFFFu;
  setBreadcrumb(kPhaseIdle);
  markStep(0);

  controller_comm::begin(controllerUart, controllerConfig);
  lcd.setup();
  keypad::begin();
  ui_init();
  harden_status_messageboxes();
  schedule_ui::begin();
  runtime_ui::begin(objects.container_content_4);
  init_flushing_ui::begin();
  spektro_ui::begin();
  sd_storage::begin();

  // Catat penyebab reset + fase save terakhir ke SD (serial USB-CDC tidak
  // reliable melewati reset). Buka /boot_log.txt di PC untuk membacanya.
  {
    char logBuf[128];
    snprintf(logBuf, sizeof(logBuf),
             "boot millis=%lu reset_reason=%d last_save_phase=%ld last_loop_step=%ld",
             static_cast<unsigned long>(millis()), resetReason,
             static_cast<long>(static_cast<int32_t>(lastPhase)),
             static_cast<long>(static_cast<int32_t>(lastStep)));
    sd_storage::appendBootLog(String(logBuf));
  }

  loadScreen(SCREEN_ID_MENU_INTRO);
  refresh_all_ui();
  lv_timer_handler();
  lv_timer_create(intro_timer_cb, 3000, nullptr);
}

void loop() {
  markStep(10);
  controller_comm::tick();
  if (controller_comm::consumeDirtyFlag()) {
    markStep(11);
    refresh_all_ui();
  }

  const app::ControllerStatus &status = controller_comm::status();
  markStep(12);
  runtime_ui::tickLoading(status);
  markStep(13);
  init_flushing_ui::tickLoading(status);

  markStep(14);
  keypad::tick(activeTextarea);

  markStep(15);
  ui_tick();
  markStep(16);
  process_sd_save_request();
  markStep(17);
  lv_timer_handler();
  markStep(18);
  delay(10);
}
