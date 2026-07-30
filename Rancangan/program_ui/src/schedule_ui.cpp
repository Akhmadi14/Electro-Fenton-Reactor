#include "schedule_ui.h"

#include <ArduinoJson.h>
#include <stdint.h>
#include <stdlib.h>

#include "controller_comm.h"
#include "runtime_ui.h"
#include "ui/actions.h"
#include "ui/screens.h"

namespace {

constexpr size_t kMaxScheduleEntries = 20;
constexpr size_t kEntriesPerPage = 5;
// =============================================================
// BATAS PARAMETER INPUT — ubah nilai-nilai di sini untuk
// menyesuaikan batasan yang diizinkan oleh sistem.
// =============================================================

// RPM magnetic stirrer: boleh 0 (mati) ATAU dalam rentang
// kMinRpmAktif – kMaxMagneticStirrerRpm.
constexpr long kMinMagneticStirrerRpm     = 0;
constexpr long kMinRpmAktif               = 100;   // batas bawah saat menyala
constexpr long kMaxMagneticStirrerRpm     = 1200;

// Suhu reaktor (°C)
constexpr float kMinSuhuC                 = 25.0f;
constexpr float kMaxSuhuC                 = 40.0f;

// Arus elektrofenton (mA)
constexpr long kMinArusMa                 = 0;
constexpr long kMaxArusMa                 = 220;

// Durasi proses
constexpr long kMinDurasiJam              = 0;
constexpr long kMaxDurasiJam              = 2;
constexpr long kMinDurasiMenit            = 0;
constexpr long kMaxDurasiMenit            = 59;

// =============================================================
constexpr lv_coord_t kWarningDialogWidth = 360;

app::ScheduleEntry scheduleEntries[kMaxScheduleEntries];
size_t scheduleCount = 0;
size_t currentPage = 0;
uint32_t scheduleRevision = 1;
uint32_t lastRenderedScheduleRevision = 0;
bool lastRenderedAllowDelete = false;
size_t lastRenderedPage = SIZE_MAX;
lv_obj_t *lastRenderedList = nullptr;
lv_obj_t *elektrofentonWarningLabel = nullptr;
lv_obj_t *adsorpsiWarningLabel = nullptr;

struct ElectroFentonInputValues {
  float suhu = 0.0f;
  long rpm = 0;
  long arus = 0;
  long jam = 0;
  long menit = 0;
  long durasiDetik = 0;
  long airflowLevel = 0;
  bool ukurDegradasi = false;
};

struct AdsorpsiInputValues {
  float suhu = 0.0f;
  long rpm = 0;
  long jam = 0;
  long menit = 0;
  long durasiDetik = 0;
  bool ukurDegradasi = false;
};

const char *get_text_or_default(lv_obj_t *textarea, const char *fallback) {
  if (!textarea) {
    return fallback;
  }

  const char *text = lv_textarea_get_text(textarea);
  return (text && text[0] != '\0') ? text : fallback;
}

long parse_long_or_default(const char *text, long fallback) {
  if (!text || text[0] == '\0') {
    return fallback;
  }

  char *endPtr = nullptr;
  const long value = strtol(text, &endPtr, 10);
  return endPtr != text ? value : fallback;
}

float parse_float_or_default(const char *text, float fallback) {
  if (!text || text[0] == '\0') {
    return fallback;
  }

  char *endPtr = nullptr;
  const float value = strtof(text, &endPtr);
  return endPtr != text ? value : fallback;
}

long read_long_textarea(lv_obj_t *textarea, long fallback) {
  return parse_long_or_default(get_text_or_default(textarea, ""), fallback);
}

float read_float_textarea(lv_obj_t *textarea, float fallback) {
  return parse_float_or_default(get_text_or_default(textarea, ""), fallback);
}

long read_duration_seconds(lv_obj_t *jamTextarea, lv_obj_t *menitTextarea) {
  const long jam = read_long_textarea(jamTextarea, 0);
  const long menit = read_long_textarea(menitTextarea, 0);
  return (jam * 3600L) + (menit * 60L);
}

lv_obj_t *get_toggle_from_container(lv_obj_t *container) {
  if (!container || lv_obj_get_child_cnt(container) < 2) {
    return nullptr;
  }

  return lv_obj_get_child(container, 1);
}

bool is_toggle_checked(lv_obj_t *container) {
  lv_obj_t *toggle = get_toggle_from_container(container);
  return toggle && lv_obj_has_state(toggle, LV_STATE_CHECKED);
}

long get_selected_airflow_level() {
  if (!objects.roller_input_level_airflow) {
    return 0;
  }

  return static_cast<long>(lv_roller_get_selected(objects.roller_input_level_airflow));
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

bool is_within_range(long value, long minValue, long maxValue) {
  return value >= minValue && value <= maxValue;
}

bool is_within_range(float value, float minValue, float maxValue) {
  return value >= minValue && value <= maxValue;
}

// RPM valid jika = 0 (stirrer mati) ATAU dalam rentang aktif.
bool is_rpm_valid(long rpm) {
  return rpm == kMinMagneticStirrerRpm ||
         is_within_range(rpm, kMinRpmAktif, kMaxMagneticStirrerRpm);
}

bool is_schedule_editing_locked(const app::ControllerStatus &status) {
  if (status.state != app::ControllerState::Idle) {
    return true;
  }

  return status.waitingAck &&
         status.pendingType == app::PendingCommandType::StartSchedule;
}

void update_runtime_monitor() {
  const app::ControllerStatus &status = controller_comm::status();
  runtime_ui::updateMonitor(status, scheduleCount);
}

void hide_warning_box(lv_obj_t *box) {
  set_hidden(box, true);
}

void hide_all_warning_boxes() {
  hide_warning_box(objects.messagebox_peringatan_elektrofenton);
  hide_warning_box(objects.messagebox_peringatan_adsorpsi);
}

void append_warning_line(String &message, const char *line) {
  if (message.length() == 0) {
    message = "PERINGATAN:\n";
  } else {
    message += "\n";
  }

  message += "- ";
  message += line;
}

ElectroFentonInputValues read_elektrofenton_inputs() {
  ElectroFentonInputValues input;
  input.suhu = read_float_textarea(objects.textarea_input_suhu, 0.0f);
  input.rpm = read_long_textarea(objects.textarea_input_magnetic_stirrer_speed, 0);
  input.arus = read_long_textarea(objects.textarea_input_arus, 0);
  input.jam = read_long_textarea(objects.textarea_input_jam, 0);
  input.menit = read_long_textarea(objects.textarea_input_menit, 0);
  input.durasiDetik = read_duration_seconds(objects.textarea_input_jam, objects.textarea_input_menit);
  input.airflowLevel = get_selected_airflow_level();
  input.ukurDegradasi = is_toggle_checked(objects.container_ukur_degradasi);
  return input;
}

AdsorpsiInputValues read_adsorpsi_inputs() {
  AdsorpsiInputValues input;
  input.suhu = read_float_textarea(objects.textarea_input_suhu_1, 0.0f);
  input.rpm = read_long_textarea(objects.textarea_input_magnetic_stirrer_speed_1, 0);
  input.jam = read_long_textarea(objects.textarea_input_jam_1, 0);
  input.menit = read_long_textarea(objects.textarea_input_menit_1, 0);
  input.durasiDetik = read_duration_seconds(objects.textarea_input_jam_1, objects.textarea_input_menit_1);
  input.ukurDegradasi = is_toggle_checked(objects.container_ukur_degradasi_1);
  return input;
}

String build_elektrofenton_validation_message(const ElectroFentonInputValues &input) {
  String message;

  if (!is_rpm_valid(input.rpm)) {
    char buf[80];
    snprintf(buf, sizeof(buf),
             "RPM magnetic stirrer harus 0 (mati) atau %ld-%ld.",
             kMinRpmAktif, kMaxMagneticStirrerRpm);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.suhu, kMinSuhuC, kMaxSuhuC)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Suhu harus dalam rentang %.0f-%.0f C.",
             kMinSuhuC, kMaxSuhuC);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.arus, kMinArusMa, kMaxArusMa)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Arus harus dalam rentang %ld-%ld mA.",
             kMinArusMa, kMaxArusMa);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.jam, kMinDurasiJam, kMaxDurasiJam)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Jam harus dalam rentang %ld-%ld.",
             kMinDurasiJam, kMaxDurasiJam);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.menit, kMinDurasiMenit, kMaxDurasiMenit)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Menit harus dalam rentang %ld-%ld.",
             kMinDurasiMenit, kMaxDurasiMenit);
    append_warning_line(message, buf);
  }

  return message;
}

String build_adsorpsi_validation_message(const AdsorpsiInputValues &input) {
  String message;

  if (!is_rpm_valid(input.rpm)) {
    char buf[80];
    snprintf(buf, sizeof(buf),
             "RPM magnetic stirrer harus 0 (mati) atau %ld-%ld.",
             kMinRpmAktif, kMaxMagneticStirrerRpm);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.suhu, kMinSuhuC, kMaxSuhuC)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Suhu harus dalam rentang %.0f-%.0f C.",
             kMinSuhuC, kMaxSuhuC);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.jam, kMinDurasiJam, kMaxDurasiJam)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Jam harus dalam rentang %ld-%ld.",
             kMinDurasiJam, kMaxDurasiJam);
    append_warning_line(message, buf);
  }

  if (!is_within_range(input.menit, kMinDurasiMenit, kMaxDurasiMenit)) {
    char buf[64];
    snprintf(buf, sizeof(buf), "Menit harus dalam rentang %ld-%ld.",
             kMinDurasiMenit, kMaxDurasiMenit);
    append_warning_line(message, buf);
  }

  return message;
}

void show_warning_box(lv_obj_t *box, lv_obj_t *label, const String &message) {
  if (!box || !label) {
    return;
  }

  lv_label_set_text(label, message.c_str());
  set_hidden(box, false);
  lv_obj_move_foreground(box);
}

void configure_warning_box(lv_obj_t *box,
                           lv_obj_t *generatedLabel,
                           lv_obj_t **runtimeLabel,
                           lv_event_cb_t closeAction) {
  if (!box) {
    return;
  }

  if (generatedLabel) {
    lv_obj_add_flag(generatedLabel, LV_OBJ_FLAG_HIDDEN);
  }

  lv_obj_set_width(box, kWarningDialogWidth);
  lv_obj_set_height(box, LV_SIZE_CONTENT);

  lv_obj_t *content = lv_msgbox_get_content(box);
  if (content) {
    lv_obj_set_style_pad_left(content, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(content, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(content, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(content, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  }

  lv_obj_t *closeButton = lv_msgbox_get_close_btn(box);
  if (closeButton) {
    lv_obj_remove_event_cb(closeButton, NULL);
    lv_obj_add_event_cb(closeButton, closeAction, LV_EVENT_CLICKED, nullptr);
  }

  if (!*runtimeLabel && content) {
    *runtimeLabel = lv_label_create(content);
    lv_obj_set_width(*runtimeLabel, LV_PCT(100));
    lv_label_set_long_mode(*runtimeLabel, LV_LABEL_LONG_WRAP);
    lv_obj_set_style_text_color(*runtimeLabel, lv_color_hex(0xff5f2120),
                                LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(*runtimeLabel, "");
  }

  set_hidden(box, true);
}

void initialize_warning_boxes() {
  configure_warning_box(objects.messagebox_peringatan_elektrofenton,
                        objects.pesan_peringatan,
                        &elektrofentonWarningLabel,
                        action_tutup_pesan_error_input_elektrofenton);
  configure_warning_box(objects.messagebox_peringatan_adsorpsi,
                        objects.pesan_peringatan_1,
                        &adsorpsiWarningLabel,
                        action_tutup_pesan_error_input_adsorpsi);
}

const char *schedule_title(app::ReactionType reaction) {
  switch (reaction) {
    case app::ReactionType::ElectroFenton:    return "Elektrofenton";
    case app::ReactionType::Adsorpsi:         return "Adsorpsi";
    case app::ReactionType::Spektrofotometri: return "Spektrofotometri";
    default:                                  return "Proses";
  }
}

String build_schedule_details(const app::ScheduleEntry &entry) {
  if (entry.reaction == app::ReactionType::Spektrofotometri) {
    return String("Pengukuran Spektrum");
  }

  const String suhuText = app::formatFloatValue(entry.suhu);
  const String durasiText = app::formatDurationHhMmSs(entry.durasiDetik);
  char details[192];

  if (entry.reaction == app::ReactionType::ElectroFenton) {
    snprintf(details, sizeof(details),
             "RPM %ld | Suhu %s C | Arus %ld mA | %s | Airflow %ld",
             entry.rpm, suhuText.c_str(), entry.arus,
             durasiText.c_str(), entry.airflowLevel);
  } else {
    snprintf(details, sizeof(details),
             "RPM %ld | Suhu %s C | Durasi %s",
             entry.rpm, suhuText.c_str(), durasiText.c_str());
  }

  return String(details);
}

String build_schedule_line(const app::ScheduleEntry &entry) {
  String line = schedule_title(entry.reaction);
  line += " | ";
  line += build_schedule_details(entry);
  return line;
}

bool append_schedule_entry(const app::ScheduleEntry &entry) {
  if (scheduleCount >= kMaxScheduleEntries) {
    Serial.println("Storage jadwal penuh, entri baru diabaikan.");
    return false;
  }

  scheduleEntries[scheduleCount++] = entry;
  ++scheduleRevision;
  return true;
}

void delete_schedule_entry(size_t index) {
  if (index >= scheduleCount) {
    return;
  }

  for (size_t itemIndex = index + 1; itemIndex < scheduleCount; ++itemIndex) {
    scheduleEntries[itemIndex - 1] = scheduleEntries[itemIndex];
  }

  --scheduleCount;
  ++scheduleRevision;
}

void update_schedule_action_visibility(const app::ControllerStatus &status) {
  const bool hidden = is_schedule_editing_locked(status);
  set_hidden(objects.button_start_jadwal, hidden);
  set_hidden(objects.button_tambah, hidden);
}

void add_schedule_entry_to_list(size_t index, const app::ScheduleEntry &entry, bool allowDelete) {
  if (!objects.list_jadwal) {
    return;
  }

  lv_obj_t *row = lv_obj_create(objects.list_jadwal);
  lv_obj_set_size(row, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_clear_flag(row, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_left(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(row, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_t *label = lv_label_create(row);
  lv_obj_set_flex_grow(label, 1);
  lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
  lv_label_set_text(label, build_schedule_line(entry).c_str());

  if (!allowDelete) {
    return;
  }

  lv_obj_t *deleteButton = lv_btn_create(row);
  lv_obj_set_size(deleteButton, 84, 38);
  lv_obj_set_style_bg_color(deleteButton, lv_color_hex(0xffd32f2f),
                            LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_add_event_cb(
      deleteButton,
      [](lv_event_t *e) {
        lv_async_call(
            [](void *userData) {
              if (is_schedule_editing_locked(controller_comm::status())) {
                return;
              }

              const size_t indexToDelete =
                  static_cast<size_t>(reinterpret_cast<uintptr_t>(userData));
              delete_schedule_entry(indexToDelete);
              schedule_ui::sync(controller_comm::status());
              update_runtime_monitor();
            },
            lv_event_get_user_data(e));
      },
      LV_EVENT_CLICKED,
      reinterpret_cast<void *>(static_cast<uintptr_t>(index)));

  lv_obj_t *deleteLabel = lv_label_create(deleteButton);
  lv_label_set_text(deleteLabel, "Hapus");
  lv_obj_center(deleteLabel);
}

void render_schedule_list() {
  if (!objects.list_jadwal) {
    return;
  }

  size_t pageCount = (scheduleCount == 0) ? 1 :
      ((scheduleCount + kEntriesPerPage - 1) / kEntriesPerPage);
  if (currentPage >= pageCount) {
    currentPage = pageCount - 1;
  }

  const bool allowDelete = !is_schedule_editing_locked(controller_comm::status());
  if (lastRenderedList == objects.list_jadwal &&
      lastRenderedScheduleRevision == scheduleRevision &&
      lastRenderedAllowDelete == allowDelete &&
      lastRenderedPage == currentPage) {
    return;
  }

  lv_obj_clean(objects.list_jadwal);
  lv_obj_clear_flag(objects.list_jadwal, LV_OBJ_FLAG_SCROLLABLE);

  size_t startIndex = currentPage * kEntriesPerPage;
  size_t endIndex = startIndex + kEntriesPerPage;
  if (endIndex > scheduleCount) {
    endIndex = scheduleCount;
  }

  for (size_t index = startIndex; index < endIndex; ++index) {
    add_schedule_entry_to_list(index, scheduleEntries[index], allowDelete);
  }

  if (objects.teks_jumlah_halaman) {
    char buf[16];
    snprintf(buf, sizeof(buf), "%u/%u",
             (unsigned)(currentPage + 1), (unsigned)pageCount);
    lv_label_set_text(objects.teks_jumlah_halaman, buf);
  }

  lastRenderedList = objects.list_jadwal;
  lastRenderedScheduleRevision = scheduleRevision;
  lastRenderedAllowDelete = allowDelete;
  lastRenderedPage = currentPage;
}

String build_start_schedule_payload() {
  JsonDocument doc;
  doc["status"] = "start";

  JsonArray jadwal = doc["jadwal"].to<JsonArray>();
  for (size_t index = 0; index < scheduleCount; ++index) {
    const app::ScheduleEntry &entry = scheduleEntries[index];
    JsonObject proses = jadwal.add<JsonObject>();

    proses["reaksi"] = app::reactionToJsonName(entry.reaction);

    if (entry.reaction != app::ReactionType::Spektrofotometri) {
      proses["suhu"] = entry.suhu;
      proses["rpm"] = entry.rpm;
      proses["durasi"] = entry.durasiDetik;
      proses["ukur_degradasi"] = entry.ukurDegradasi;

      if (entry.reaction == app::ReactionType::ElectroFenton) {
        proses["arus"] = entry.arus;
        proses["airflow_level"] = entry.airflowLevel;
      }
    }
  }

  String payload;
  serializeJson(doc, payload);
  return payload;
}

}  // namespace

namespace schedule_ui {

void begin() {
  initialize_warning_boxes();
  sync(controller_comm::status());
}

void sync(const app::ControllerStatus &status) {
  update_schedule_action_visibility(status);
  render_schedule_list();
}

void hideWarningBoxes() {
  hide_all_warning_boxes();
}

bool addElectroFenton() {
  if (is_schedule_editing_locked(controller_comm::status())) {
    Serial.println("Perubahan jadwal dikunci saat START menunggu ACK atau proses sedang berjalan.");
    return false;
  }

  const ElectroFentonInputValues input = read_elektrofenton_inputs();
  const String validationMessage = build_elektrofenton_validation_message(input);
  if (validationMessage.length() > 0) {
    show_warning_box(objects.messagebox_peringatan_elektrofenton,
                     elektrofentonWarningLabel,
                     validationMessage);
    return false;
  }

  app::ScheduleEntry entry;
  entry.reaction = app::ReactionType::ElectroFenton;
  entry.suhu = input.suhu;
  entry.rpm = input.rpm;
  entry.arus = input.arus;
  entry.durasiDetik = input.durasiDetik;
  entry.airflowLevel = input.airflowLevel;
  entry.ukurDegradasi = input.ukurDegradasi;

  if (!append_schedule_entry(entry)) {
    show_warning_box(objects.messagebox_peringatan_elektrofenton,
                     elektrofentonWarningLabel,
                     "PERINGATAN:\n- Kapasitas jadwal penuh. Hapus salah satu jadwal terlebih dahulu.");
    return false;
  }

  hide_warning_box(objects.messagebox_peringatan_elektrofenton);
  sync(controller_comm::status());
  update_runtime_monitor();
  return true;
}

bool addAdsorpsi() {
  if (is_schedule_editing_locked(controller_comm::status())) {
    Serial.println("Perubahan jadwal dikunci saat START menunggu ACK atau proses sedang berjalan.");
    return false;
  }

  const AdsorpsiInputValues input = read_adsorpsi_inputs();
  const String validationMessage = build_adsorpsi_validation_message(input);
  if (validationMessage.length() > 0) {
    show_warning_box(objects.messagebox_peringatan_adsorpsi,
                     adsorpsiWarningLabel,
                     validationMessage);
    return false;
  }

  app::ScheduleEntry entry;
  entry.reaction = app::ReactionType::Adsorpsi;
  entry.suhu = input.suhu;
  entry.rpm = input.rpm;
  entry.durasiDetik = input.durasiDetik;
  entry.ukurDegradasi = input.ukurDegradasi;

  if (!append_schedule_entry(entry)) {
    show_warning_box(objects.messagebox_peringatan_adsorpsi,
                     adsorpsiWarningLabel,
                     "PERINGATAN:\n- Kapasitas jadwal penuh. Hapus salah satu jadwal terlebih dahulu.");
    return false;
  }

  hide_warning_box(objects.messagebox_peringatan_adsorpsi);
  sync(controller_comm::status());
  update_runtime_monitor();
  return true;
}

bool addSpektrofotometri() {
  if (is_schedule_editing_locked(controller_comm::status())) {
    Serial.println("Perubahan jadwal dikunci saat proses sedang berjalan.");
    return false;
  }

  app::ScheduleEntry entry;
  entry.reaction = app::ReactionType::Spektrofotometri;

  if (!append_schedule_entry(entry)) {
    // Show the ElektroFenton warning box — it's the only reusable warning
    // widget accessible from this context. Without this the UI is silent.
    show_warning_box(objects.messagebox_peringatan_elektrofenton,
                     elektrofentonWarningLabel,
                     "PERINGATAN:\n- Kapasitas jadwal penuh. Hapus salah satu jadwal terlebih dahulu.");
    return false;
  }

  sync(controller_comm::status());
  update_runtime_monitor();
  return true;
}

bool startSend() {
  const app::ControllerStatus &status = controller_comm::status();
  if (is_schedule_editing_locked(status)) {
    Serial.println("START diabaikan karena jadwal sedang dikirim atau proses sedang berjalan.");
    return false;
  }

  if (scheduleCount == 0) {
    Serial.println("Belum ada jadwal untuk dikirim.");
    return false;
  }

  return controller_comm::startSchedule(build_start_schedule_payload());
}

size_t count() {
  return scheduleCount;
}

void resetSchedule() {
  scheduleCount = 0;
  currentPage = 0;
  ++scheduleRevision;
  render_schedule_list();
}

void nextPage() {
  size_t pageCount = (scheduleCount == 0) ? 1 :
      ((scheduleCount + kEntriesPerPage - 1) / kEntriesPerPage);
  if (currentPage + 1 < pageCount) {
    ++currentPage;
    render_schedule_list();
  }
}

void backPage() {
  if (currentPage > 0) {
    --currentPage;
    render_schedule_list();
  }
}

size_t entryCount() {
  return scheduleCount;
}

const app::ScheduleEntry &entryAt(size_t index) {
  static const app::ScheduleEntry kEmpty;
  if (index >= scheduleCount) {
    return kEmpty;
  }
  return scheduleEntries[index];
}

}  // namespace schedule_ui

extern "C" {

void action_tutup_pesan_error_input_elektrofenton(lv_event_t *e) {
  (void)e;
  hide_warning_box(objects.messagebox_peringatan_elektrofenton);
}

void action_tutup_pesan_error_input_adsorpsi(lv_event_t *e) {
  (void)e;
  hide_warning_box(objects.messagebox_peringatan_adsorpsi);
}

}  // extern "C"
