#ifndef SPEKTRO_UI_H
#define SPEKTRO_UI_H

#include "app_shared.h"

namespace spektro_ui {

// Called once from setup() after ui_init().
void begin();

// Called every time the user navigates to either Spektro screen
// (Spektrum Warna or Degradasi Warna).  Must be called AFTER loadScreen()
// so the chart widget pointer is valid.  Flushes any deferred data that
// arrived while the screen was not active.
void onSpektroScreenActivated();

// Called from controller_comm when a degradasi measurement arrives.
// id         : process id from JSON "id" field
// reaction   : ElectroFenton or Adsorpsi
// t_menit    : time in minutes
// absorbansi : raw absorbance from sensor
void onDegradasiPoint(int id, app::ReactionType reaction,
                      int t_menit, float absorbansi);

// Called from controller_comm when a full spektrum array arrives.
// id    : process id from JSON "id" field
// data  : array of absorbance values from 400nm (index 0) to 800nm (index 400)
// count : number of valid values in data (should be 401)
void onSpektrumReceived(int id, const float *data, int count);

// Reset all chart data and list entries (called after flushing etc.)
void resetAll();

// ---- Read-only export views for SD card persistence ----
// Pointers are valid only until the next call into spektro_ui that
// mutates the underlying slot arrays. Treat as snapshots.

struct SpektroExport {
  int          id   = 0;
  const float *data = nullptr;
  int          len  = 0;     // jumlah elemen valid (umumnya 401, 400..800nm)
};

struct DegradasiExport {
  int               id      = 0;
  app::ReactionType reaction = app::ReactionType::None;
  const int        *tMin    = nullptr;  // menit ke-i
  const float      *absVal  = nullptr;  // absorbansi ke-i
  int               count   = 0;
};

int collectActiveSpektro(SpektroExport *out, int maxOut);
int collectActiveDegradasi(DegradasiExport *out, int maxOut);

}  // namespace spektro_ui

#endif  // SPEKTRO_UI_H
