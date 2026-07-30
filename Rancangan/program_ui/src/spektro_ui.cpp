#include "spektro_ui.h"

#include <lvgl.h>
#include <stdio.h>

#include "ui/screens.h"
#include "ui/ui.h"

namespace {

constexpr int kMaxProc       = 16;
constexpr int kMaxPts        = 60;
constexpr int kMaxSpektro    = 16;
constexpr int kWavelengths   = 128;  // 128 pixel sensor, mapped 400–800 nm

constexpr lv_coord_t kSpektroYMax  = 300;   // 3.0 absorbansi × 100
constexpr lv_coord_t kSpektroMinVisibleRange = 25;  // 0.25 absorbansi × 100
constexpr lv_coord_t kSpektroScale = 100;   // display as X.XX absorbansi
constexpr lv_coord_t kDegradaciPointSize = 8;

// 16-colour sequential palette: hue sweeps evenly 0°→337.5° (red→orange→
// yellow→green→cyan→blue→indigo→violet) so among 12+ entries the eye can
// tell first (warm) from last (cool/violet) and middle intuitively.
static lv_color_t paletteColor(int idx) {
  // Pre-computed HSL(hue, 75%, 48%) → sRGB hex for 16 equidistant hues.
  static const uint32_t kColors[16] = {
    0xd62626u,  //  0°  merah
    0xd65f26u,  // 22°  oranye-merah
    0xd69926u,  // 45°  oranye
    0xd6cc26u,  // 67°  kuning-oranye
    0x99d626u,  // 90°  kuning-hijau
    0x4fd626u,  // 112° hijau muda
    0x26d662u,  // 135° hijau
    0x26d6b2u,  // 157° tosca
    0x26c5d6u,  // 180° cyan
    0x268dd6u,  // 202° biru muda
    0x2655d6u,  // 225° biru
    0x4526d6u,  // 247° biru-ungu
    0x8026d6u,  // 270° ungu
    0xb826d6u,  // 292° ungu-merah muda
    0xd626a8u,  // 315° pink
    0xd62671u,  // 337° merah muda
  };
  return lv_color_hex(kColors[idx % 16]);
}

struct DegradasiProc {
  bool active      = false;
  int  id          = 0;
  app::ReactionType reaction = app::ReactionType::None;
  float baseLine   = 0.0f;
  int   tMin[kMaxPts];
  float absVal[kMaxPts];
  int   count      = 0;
  bool  visible    = true;
  lv_chart_series_t *ser    = nullptr;
  lv_obj_t          *listBtn = nullptr;
};

struct SpektroEntry {
  bool  active     = false;
  int   id         = 0;
  float data[kWavelengths];
  bool  visible    = true;
  lv_chart_series_t *ser    = nullptr;
  lv_obj_t          *listBtn = nullptr;
};

static DegradasiProc degradasiSlots[kMaxProc];
static SpektroEntry  spektroSlots[kMaxSpektro];

// True when onSpektrumReceived() received data while the spektro screen was
// not active.  onSpektroScreenActivated() will flush the pending rebuild.
static bool pendingRebuildSpektro = false;

// True after the spektro axis-label event callback has been registered once.
// LVGL does NOT deduplicate lv_obj_add_event_cb — calling it multiple times
// adds duplicate callbacks that all fire during draw, corrupting the text
// buffer.  This flag ensures we only add it once.
static bool spektroEventCbRegistered = false;

// Returns true when the spektrum chart's screen is the currently loaded one.
static bool isSpektroScreenActive() {
  lv_obj_t *act = lv_scr_act();
  return act == objects.menu_spektro_spektrum_warna ||
         act == objects.menu_spektro_degradasi_warna;
}

static const char *procShortName(app::ReactionType r) {
  if (r == app::ReactionType::ElectroFenton) return "EF";
  if (r == app::ReactionType::Adsorpsi)      return "Adsorpsi";
  return "?";
}

static int findDegradaciSlot(int id) {
  for (int i = 0; i < kMaxProc; i++)
    if (degradasiSlots[i].active && degradasiSlots[i].id == id) return i;
  return -1;
}

static int findOrAllocDegradaciSlot(int id) {
  int found = findDegradaciSlot(id);
  if (found >= 0) return found;
  for (int i = 0; i < kMaxProc; i++)
    if (!degradasiSlots[i].active) return i;
  return -1;
}

static int findOrAllocSpektroSlot(int id) {
  for (int i = 0; i < kMaxSpektro; i++)
    if (spektroSlots[i].active && spektroSlots[i].id == id) return i;
  for (int i = 0; i < kMaxSpektro; i++)
    if (!spektroSlots[i].active) return i;
  return -1;
}

// Max time across all active degradasi processes
static int globalMaxTMin() {
  int mx = 0;
  for (int i = 0; i < kMaxProc; i++) {
    if (!degradasiSlots[i].active) continue;
    for (int j = 0; j < degradasiSlots[i].count; j++) {
      if (degradasiSlots[i].tMin[j] > mx) mx = degradasiSlots[i].tMin[j];
    }
  }
  return mx;
}

// Max data point count across all active degradasi processes
static int globalMaxPtCount() {
  int mx = 0;
  for (int i = 0; i < kMaxProc; i++) {
    if (degradasiSlots[i].active && degradasiSlots[i].count > mx)
      mx = degradasiSlots[i].count;
  }
  return mx;
}

// Compute Y-axis max: global maximum absorbansi across all active spektro slots,
// then add 10% headroom. Values are stored as float absorbansi and displayed
// scaled by kSpektroScale (×100) as integers to LVGL.
static lv_coord_t computeSpektroRangeMax() {
  lv_coord_t observedMax = 0;
  for (int s = 0; s < kMaxSpektro; s++) {
    if (!spektroSlots[s].active) continue;
    for (int j = 0; j < kWavelengths; j++) {
      lv_coord_t value =
          static_cast<lv_coord_t>(spektroSlots[s].data[j] * kSpektroScale);
      if (value > observedMax) {
        observedMax = value;
      }
    }
  }

  if (observedMax < kSpektroMinVisibleRange) {
    return kSpektroMinVisibleRange;
  }

  // +10% headroom, exact (no rounding)
  return observedMax + observedMax / 10;
}

// ---- Shape customization for scatter points ----
// Circle for even-indexed series, square for odd, rounded for idx>=2
static lv_coord_t shapeRadius(int seriesIdx) {
  switch (seriesIdx % 4) {
    case 0: return LV_RADIUS_CIRCLE;  // circle
    case 1: return 0;                 // square
    case 2: return 2;                 // rounded square
    case 3: return LV_RADIUS_CIRCLE;  // circle (smaller, set in size)
    default: return LV_RADIUS_CIRCLE;
  }
}

// Find which slot index a series pointer belongs to
static int degradaciSeriesIndex(const lv_chart_series_t *ser) {
  for (int i = 0; i < kMaxProc; i++) {
    if (degradasiSlots[i].active && degradasiSlots[i].ser == ser) return i;
  }
  return -1;
}

// Draw event: customize point shapes per series
static void degradaciDrawEvent(lv_event_t *e) {
  lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
  if (!dsc || dsc->part != LV_PART_INDICATOR) return;

  const lv_chart_series_t *ser = static_cast<const lv_chart_series_t *>(dsc->sub_part_ptr);
  int idx = degradaciSeriesIndex(ser);
  if (idx < 0) return;

  if (dsc->rect_dsc) {
    dsc->rect_dsc->radius = shapeRadius(idx);
  }
}

// Draw event: format Y-axis tick labels as absorbansi float "X.XX".
// X-axis ticks are wavelengths (400-800 nm) and are left as-is.
// Y-axis ticks are absorbansi × kSpektroScale (<400), formatted as float.
static void spektroAxisLabelEvent(lv_event_t *e) {
  lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
  if (!dsc || dsc->part != LV_PART_TICKS) return;
  if (!dsc->text || dsc->text_length == 0) return;
  // Y axis values are absorbansi×100 (range 0..~300).
  // X axis values are wavelength nm (range 400..800).
  // Distinguish by value: Y < 400, X >= 400.
  if (dsc->value < 400) {
    snprintf(dsc->text, dsc->text_length, "%.2f",
             (float)dsc->value / (float)kSpektroScale);
  }
}

// Compute Y-axis max for degradasi chart: max percentage across all active slots
// (visible or not), rounded up to the nearest 10, minimum 110.
static lv_coord_t computeDegradaciRangeMax() {
  lv_coord_t observedMax = 0;
  for (int i = 0; i < kMaxProc; i++) {
    const DegradasiProc &p = degradasiSlots[i];
    if (!p.active || p.baseLine <= 0.0f) continue;
    for (int j = 0; j < p.count; j++) {
      lv_coord_t val =
          static_cast<lv_coord_t>((p.absVal[j] / p.baseLine) * 100.0f);
      if (val > observedMax) observedMax = val;
    }
  }
  lv_coord_t rounded = ((observedMax + 9) / 10) * 10;
  return (rounded < 110) ? 110 : rounded;
}

// ---- Chart rebuild ----
static void rebuildDegradaciChart() {
  lv_obj_t *chart = objects.chart_degradasi_warna;
  if (!chart) return;

  int maxT = globalMaxTMin();
  if (maxT < 1) maxT = 1;

  int ptCount = globalMaxPtCount();
  if (ptCount < 2) ptCount = 2;
  if (ptCount > kMaxPts) ptCount = kMaxPts;

  lv_chart_set_point_count(chart, static_cast<uint16_t>(ptCount));

  // X range = 0..maxT (minutes), Y range = 0..dynamic (%)
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 0,
                     static_cast<lv_coord_t>(maxT));
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, computeDegradaciRangeMax());

  // X-axis ticks
  int xMajorTicks = (maxT <= 8) ? maxT + 1 : 7;
  if (xMajorTicks > 9) xMajorTicks = 9;
  if (xMajorTicks < 2) xMajorTicks = 2;
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X,
                         5, 2, xMajorTicks, 2, true, 30);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y,
                         8, 2, 7, 2, true, 40);

  for (int i = 0; i < kMaxProc; i++) {
    DegradasiProc &p = degradasiSlots[i];
    if (!p.active || !p.ser) continue;

    // Clear all points to NONE
    for (int k = 0; k < ptCount; k++) {
      lv_chart_set_value_by_id2(chart, p.ser, static_cast<uint16_t>(k),
                                 LV_CHART_POINT_NONE, LV_CHART_POINT_NONE);
    }

    if (!p.visible || p.count == 0 || p.baseLine <= 0.0f) continue;

    // Place each measurement at its exact (time, percent) coordinate
    for (int j = 0; j < p.count && j < ptCount; j++) {
      lv_coord_t xVal = static_cast<lv_coord_t>(p.tMin[j]);
      lv_coord_t yVal =
          static_cast<lv_coord_t>((p.absVal[j] / p.baseLine) * 100.0f);
      lv_chart_set_value_by_id2(chart, p.ser,
                                 static_cast<uint16_t>(j), xVal, yVal);
    }
  }
  lv_chart_refresh(chart);
}

static void rebuildSpektroChart() {
  lv_obj_t *chart = objects.chart_degradasi_warna_1;
  if (!chart) return;

  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_X, 400, 800);
  lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, computeSpektroRangeMax());
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X,
                         5, 2, 9, 2, true, 40);
  lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y,
                         8, 2, 7, 2, true, 40);

  for (int i = 0; i < kMaxSpektro; i++) {
    SpektroEntry &e = spektroSlots[i];
    if (!e.active || !e.ser) continue;

    for (int k = 0; k < kWavelengths; k++) {
      lv_chart_set_value_by_id2(chart, e.ser, static_cast<uint16_t>(k),
                                LV_CHART_POINT_NONE, LV_CHART_POINT_NONE);
    }
    if (!e.visible) continue;

    for (int j = 0; j < kWavelengths; j++) {
      // wavelength(i) = 400 + 400 * i / 127  →  data[0]=400nm, data[127]=800nm
      const lv_coord_t xNm = static_cast<lv_coord_t>(400 + 400 * j / 127);
      lv_chart_set_value_by_id2(
          chart, e.ser, static_cast<uint16_t>(j),
          xNm,
          static_cast<lv_coord_t>(e.data[j] * kSpektroScale));
    }
  }
  lv_chart_refresh(chart);
}

// ---- List button callbacks ----
static void onDegradaciListBtn(lv_event_t *e) {
  int idx = static_cast<int>(reinterpret_cast<uintptr_t>(lv_event_get_user_data(e)));
  if (idx < 0 || idx >= kMaxProc) return;
  degradasiSlots[idx].visible =
      lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
  rebuildDegradaciChart();
}

static void onSpektroListBtn(lv_event_t *e) {
  int idx = static_cast<int>(reinterpret_cast<uintptr_t>(lv_event_get_user_data(e)));
  if (idx < 0 || idx >= kMaxSpektro) return;
  spektroSlots[idx].visible =
      lv_obj_has_state(lv_event_get_target(e), LV_STATE_CHECKED);
  rebuildSpektroChart();
}

// ---- Create compact list button ----
static lv_obj_t *addListButton(lv_obj_t *list, const char *label,
                                lv_color_t color, int idx,
                                lv_event_cb_t cb) {
  if (!list) return nullptr;

  lv_obj_t *btn = lv_list_add_btn(list, nullptr, label);
  if (!btn) return nullptr;

  lv_obj_set_width(btn, LV_SIZE_CONTENT);
  lv_obj_set_style_pad_left(btn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(btn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(btn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_add_state(btn, LV_STATE_CHECKED);

  lv_obj_set_style_bg_color(btn, color,
                             LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER,
                           LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_text_color(btn, lv_color_hex(0xffffffff),
                               LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_set_style_bg_color(btn, lv_color_hex(0xffc0c0c0),
                             LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(btn, LV_OPA_COVER,
                           LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_add_event_cb(btn, cb, LV_EVENT_VALUE_CHANGED,
                      reinterpret_cast<void *>(static_cast<uintptr_t>(idx)));
  return btn;
}

static void clearAllDegradasi() {
  lv_obj_t *chart = objects.chart_degradasi_warna;
  for (int i = 0; i < kMaxProc; i++) {
    DegradasiProc &p = degradasiSlots[i];
    if (p.ser && chart) lv_chart_remove_series(chart, p.ser);
    if (p.listBtn) lv_obj_del(p.listBtn);
    p = DegradasiProc{};
  }
  if (chart) lv_chart_refresh(chart);
  if (objects.list_grafik_tersedia) lv_obj_clean(objects.list_grafik_tersedia);
}

static void clearAllSpektro() {
  lv_obj_t *chart = objects.chart_degradasi_warna_1;
  for (int i = 0; i < kMaxSpektro; i++) {
    SpektroEntry &e = spektroSlots[i];
    if (e.ser && chart) lv_chart_remove_series(chart, e.ser);
    if (e.listBtn) lv_obj_del(e.listBtn);
    e = SpektroEntry{};
  }
  if (chart) lv_chart_refresh(chart);
  if (objects.list_grafik_tersedia_1) lv_obj_clean(objects.list_grafik_tersedia_1);
}

}  // namespace

namespace spektro_ui {

void begin() {
  lv_obj_t *cd = objects.chart_degradasi_warna;
  if (cd) {
    lv_chart_set_type(cd, LV_CHART_TYPE_SCATTER);
    lv_chart_set_point_count(cd, 2);

    lv_chart_set_range(cd, LV_CHART_AXIS_PRIMARY_X, 0, 1);
    lv_chart_set_range(cd, LV_CHART_AXIS_PRIMARY_Y, 0, 110);

    lv_chart_set_axis_tick(cd, LV_CHART_AXIS_PRIMARY_Y,
                           8, 4, 6, 2, true, 40);
    lv_chart_set_axis_tick(cd, LV_CHART_AXIS_PRIMARY_X,
                           5, 3, 2, 2, true, 30);

    // Point size for scatter markers
    lv_obj_set_style_size(cd, kDegradaciPointSize, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(cd, 2, LV_PART_ITEMS);

    // Custom draw event for different point shapes
    lv_obj_add_event_cb(cd, degradaciDrawEvent,
                        LV_EVENT_DRAW_PART_BEGIN, nullptr);
  }

  lv_obj_t *cs = objects.chart_degradasi_warna_1;
  if (cs) {
    lv_chart_set_type(cs, LV_CHART_TYPE_SCATTER);
    lv_chart_set_point_count(cs, kWavelengths);
    lv_chart_set_range(cs, LV_CHART_AXIS_PRIMARY_X, 400, 800);
    lv_chart_set_range(cs, LV_CHART_AXIS_PRIMARY_Y, 0, kSpektroMinVisibleRange);

    lv_chart_set_axis_tick(cs, LV_CHART_AXIS_PRIMARY_Y,
                           8, 2, 7, 2, true, 40);
    lv_chart_set_axis_tick(cs, LV_CHART_AXIS_PRIMARY_X,
                           5, 2, 9, 2, true, 40);
    lv_obj_set_style_size(cs, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_width(cs, 2, LV_PART_ITEMS);

    // Format Y-axis tick labels as absorbansi float "X.XX".
    // Mark as registered so onSpektroScreenActivated() doesn't add it again.
    lv_obj_add_event_cb(cs, spektroAxisLabelEvent,
                        LV_EVENT_DRAW_PART_BEGIN, nullptr);
    spektroEventCbRegistered = true;
  }
}

void onDegradasiPoint(int id, app::ReactionType reaction,
                      int t_menit, float absorbansi) {
  int idx = findOrAllocDegradaciSlot(id);
  if (idx < 0) return;

  DegradasiProc &p = degradasiSlots[idx];

  if (!p.active) {
    p          = DegradasiProc{};
    p.active   = true;
    p.id       = id;
    p.reaction = reaction;
    p.visible  = true;

    lv_obj_t *chart = objects.chart_degradasi_warna;
    if (chart) {
      p.ser = lv_chart_add_series(chart, paletteColor(idx),
                                  LV_CHART_AXIS_PRIMARY_Y);
    }

    char label[32];
    snprintf(label, sizeof(label), "%s(%d)", procShortName(reaction), id);
    p.listBtn = addListButton(objects.list_grafik_tersedia, label,
                               paletteColor(idx), idx, onDegradaciListBtn);
  }

  // Same id with time restarting from 0 means a new run.
  if (p.count > 0 && (t_menit == 0 || t_menit < p.tMin[p.count - 1])) {
    p.count = 0;
    p.baseLine = 0.0f;
  }

  int existingPointIndex = -1;
  for (int i = 0; i < p.count; i++) {
    if (p.tMin[i] == t_menit) {
      existingPointIndex = i;
      break;
    }
  }

  if (existingPointIndex >= 0) {
    p.absVal[existingPointIndex] = absorbansi;
    if (existingPointIndex == 0) {
      p.baseLine = absorbansi;
    }
  } else if (p.count < kMaxPts) {
    if (p.count == 0) p.baseLine = absorbansi;
    p.tMin[p.count]   = t_menit;
    p.absVal[p.count] = absorbansi;
    p.count++;
  }
 
  rebuildDegradaciChart();
}

void onSpektrumReceived(int id, const float *data, int count) {
  int idx = findOrAllocSpektroSlot(id);
  if (idx < 0) return;

  SpektroEntry &e = spektroSlots[idx];

  if (!e.active) {
    e        = SpektroEntry{};
    e.active  = true;
    e.id      = id;
    e.visible = true;

    lv_obj_t *chart = objects.chart_degradasi_warna_1;
    if (chart) {
      e.ser = lv_chart_add_series(chart, paletteColor(idx),
                                  LV_CHART_AXIS_PRIMARY_Y);
    }

    char label[24];
    snprintf(label, sizeof(label), "Spektro(%d)", id);
    e.listBtn = addListButton(objects.list_grafik_tersedia_1, label,
                               paletteColor(idx), idx, onSpektroListBtn);
  }

  const int n = (count < kWavelengths) ? count : kWavelengths;
  for (int i = 0; i < n; i++)            e.data[i] = data[i];
  for (int i = n; i < kWavelengths; i++) e.data[i] = 0.0f;

  // Only rebuild immediately when the spektro screen is visible.
  // If user is on another screen (e.g. Monitor), defer — calling
  // lv_chart_* on a background screen during an animation transition
  // can corrupt point buffers, causing the spectrum to appear cut or spiked.
  if (isSpektroScreenActive()) {
    pendingRebuildSpektro = false;
    rebuildSpektroChart();
  } else {
    pendingRebuildSpektro = true;
  }
}

// Called every time the user navigates to either spektro screen.
// Re-attaches any series whose pointer became stale, then rebuilds the chart.
// IMPORTANT: do NOT call lv_chart_set_type() here — calling it on a chart
// that already has series resets all internal point buffers in LVGL 8.x,
// corrupting the data.  Type + style is set once by begin().
void onSpektroScreenActivated() {
  lv_obj_t *cs = objects.chart_degradasi_warna_1;
  if (cs) {
    // Register the axis-label event callback exactly once.
    // LVGL does NOT deduplicate lv_obj_add_event_cb — every call appends
    // another copy.  Duplicate callbacks all fire during draw, overwriting
    // the same dsc->text buffer multiple times → corrupted axis labels.
    if (!spektroEventCbRegistered) {
      lv_obj_add_event_cb(cs, spektroAxisLabelEvent,
                          LV_EVENT_DRAW_PART_BEGIN, nullptr);
      spektroEventCbRegistered = true;
    }

    // Re-attach series for any slot that received data while the screen was
    // in the background and the chart had not yet been visited.
    for (int i = 0; i < kMaxSpektro; i++) {
      SpektroEntry &e = spektroSlots[i];
      if (!e.active) continue;
      if (!e.ser) {
        e.ser = lv_chart_add_series(cs, paletteColor(i),
                                    LV_CHART_AXIS_PRIMARY_Y);
      }
    }
  }

  // Always rebuild on entry — data may have changed while on another screen,
  // or the Y-axis range may need updating after new measurements arrived.
  pendingRebuildSpektro = false;
  rebuildSpektroChart();
}

void resetAll() {
  clearAllDegradasi();
  clearAllSpektro();
  pendingRebuildSpektro = false;
  spektroEventCbRegistered = false;
}

int collectActiveSpektro(SpektroExport *out, int maxOut) {
  if (!out || maxOut <= 0) return 0;
  int n = 0;
  for (int i = 0; i < kMaxSpektro && n < maxOut; ++i) {
    const SpektroEntry &e = spektroSlots[i];
    if (!e.active) continue;
    out[n].id   = e.id;
    out[n].data = e.data;
    out[n].len  = kWavelengths;
    ++n;
  }
  return n;
}

int collectActiveDegradasi(DegradasiExport *out, int maxOut) {
  if (!out || maxOut <= 0) return 0;
  int n = 0;
  for (int i = 0; i < kMaxProc && n < maxOut; ++i) {
    const DegradasiProc &p = degradasiSlots[i];
    if (!p.active) continue;
    out[n].id       = p.id;
    out[n].reaction = p.reaction;
    out[n].tMin     = p.tMin;
    out[n].absVal   = p.absVal;
    out[n].count    = p.count;
    ++n;
  }
  return n;
}

}  // namespace spektro_ui
