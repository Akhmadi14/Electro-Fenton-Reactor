#ifndef SD_STORAGE_H
#define SD_STORAGE_H

#include <Arduino.h>

namespace sd_storage {

enum class SaveStatus {
  Ok,
  NoCard,
  WriteError,
  Empty,
};

struct SaveResult {
  SaveStatus status = SaveStatus::Empty;
  String filenames;       // newline-separated list of files written
  int    fileCount = 0;
  String errorDetail;     // populated when status == WriteError
};

// Initialize SPI bus + mount SD card. Safe to call even if no card present.
// Stores mount state internally.
void begin();

// Returns true when card is currently mounted.
bool isMounted();

// Try to (re)mount the SD card. Returns true on success.
bool tryRemount();

// Append one diagnostic line to /boot_log.txt (used to capture reset reason
// reliably, since USB-CDC serial drops output across resets). No-op if card
// not mounted.
void appendBootLog(const String &line);

// Save every active spektro slot and degradasi slot to its own CSV file.
// Per-process filename: <ts>_<reaksi><id>.csv (reaksi = spektro|ef|adsorpsi).
// Header includes the full jadwal_reaksi block followed by a CSV table.
SaveResult saveAllSpektroDanDegradasi();

}  // namespace sd_storage

#endif  // SD_STORAGE_H
