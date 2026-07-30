#include "sd_storage.h"

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

#include "app_shared.h"
#include "schedule_ui.h"
#include "spektro_ui.h"

namespace {

// =============================================================
// SD SPI PIN ASSIGNMENT
// CrowPanel ESP32 HMI 7.0-inch (DIS08070H) per Elecrow wiki:
//   MOSI=IO11, MISO=IO13, CLK=IO12, CS=IO10
// Ubah disini kalau wiring fisik berbeda.
//
// Catatan Elecrow "CS(3.3V)": itu biasanya berarti jalur CS
// dipull-up ke 3.3V (default nonaktif). Untuk SPI tetap perlu
// GPIO yang menarik CS ke LOW saat akses. Jika slot SD tidak
// terhubung ke GPIO CS di board, mode SPI tidak akan jalan
// tanpa mod hardware (atau ganti ke SD_MMC/SDIO).
// =============================================================
constexpr int      kSdSckPin   = 12;
constexpr int      kSdMisoPin  = 13;
constexpr int      kSdMosiPin  = 11;
constexpr int      kSdCsPin    = 10;
constexpr uint32_t kSdSpiFreq  = 10000000;  // 10 MHz (turun dari 25 MHz:
                                            // kurangi SPI instability +
                                            // lonjakan arus saat tulis SD)

// =============================================================
// SAVE_AXIS_MODE: WAVELENGTH (400..800 nm, 401 titik)
// Untuk pindah ke pixel-mode (mis. 0..127):
//   1. Ganti kSpektroFirstNm/kSpektroLastNm jadi index pixel.
//   2. Ganti header CSV "wavelength,Absorbance" -> "Pixel,Absorbance"
//      di writeSpektroFile().
//   3. Pastikan spektro_ui::collectActiveSpektro mengisi data
//      dengan jumlah elemen yang konsisten (lihat kWavelengths).
// =============================================================
constexpr int kSpektroFirstNm = 400;
constexpr int kSpektroLastNm  = 800;

constexpr int kMaxSpektroOut   = 16;
constexpr int kMaxDegradasiOut = 16;

bool sdMounted = false;
bool sdBeginAttempted = false;
SPIClass *sdSpi = nullptr;

// =============================================================
// Timestamp prefix.
// TODO: ganti dengan helper RTC sungguhan (DS3231 / NTP).
// Saat ini: 12-digit angka acak, tetap berbentuk "yyyymmddhhmm"
// agar layout filename mengikuti spek user.
// =============================================================
String currentTimestampPrefix() {
  uint32_t r1 = static_cast<uint32_t>(esp_random());
  uint32_t r2 = static_cast<uint32_t>(esp_random());
  char buf[13];
  snprintf(buf, sizeof(buf), "%06lu%06lu",
           static_cast<unsigned long>(r1 % 1000000UL),
           static_cast<unsigned long>(r2 % 1000000UL));
  return String(buf);
}

const char *reactionShortName(app::ReactionType reaction) {
  switch (reaction) {
    case app::ReactionType::ElectroFenton:    return "ef";
    case app::ReactionType::Adsorpsi:         return "adsorpsi";
    case app::ReactionType::Spektrofotometri: return "spektro";
    default:                                  return "unknown";
  }
}

String formatScheduleEntryLine(const app::ScheduleEntry &entry, size_t idx) {
  // Output mengikuti contoh user persis:
  //   -spektro1
  //   -adsorpsi2,rpm=300,suhu=40,durasii=134menit
  //   -ef4,rpm=0,suhu=25,arus=200mA,aerator=3,durasi=60menit
  String line;
  line.reserve(96);
  line += '-';
  line += reactionShortName(entry.reaction);
  line += String(static_cast<unsigned long>(idx + 1));

  if (entry.reaction == app::ReactionType::Spektrofotometri) {
    return line;
  }

  const long durationMin = (entry.durasiDetik + 59L) / 60L;

  if (entry.reaction == app::ReactionType::ElectroFenton) {
    char buf[128];
    snprintf(buf, sizeof(buf),
             ",rpm=%ld,suhu=%s,arus=%ldmA,aerator=%ld,durasi=%ldmenit",
             entry.rpm,
             app::formatFloatValue(entry.suhu).c_str(),
             entry.arus,
             entry.airflowLevel,
             durationMin);
    line += buf;
    return line;
  }

  // Adsorpsi (NB: kata "durasii" mengikuti ejaan spek user).
  char buf[128];
  snprintf(buf, sizeof(buf),
           ",rpm=%ld,suhu=%s,durasii=%ldmenit",
           entry.rpm,
           app::formatFloatValue(entry.suhu).c_str(),
           durationMin);
  line += buf;
  return line;
}

String buildJadwalBlock() {
  String block;
  block.reserve(256);
  block += "jadwal_reaksi\n";

  const size_t total = schedule_ui::entryCount();
  for (size_t i = 0; i < total; ++i) {
    block += formatScheduleEntryLine(schedule_ui::entryAt(i), i);
    block += '\n';
  }
  return block;
}

bool writeSpektroFile(const spektro_ui::SpektroExport &item,
                      const String &timestamp,
                      const String &jadwalBlock,
                      String &outFilename,
                      String &outError) {
  char fnameBuf[64];
  snprintf(fnameBuf, sizeof(fnameBuf), "/%s_spektro_%d.csv",
           timestamp.c_str(), item.id);
  outFilename = fnameBuf;

  File f = SD.open(fnameBuf, FILE_WRITE);
  if (!f) {
    outError = "open failed: ";
    outError += fnameBuf;
    return false;
  }

  f.print(jadwalBlock);
  f.print('\n');
  f.print("namareaksi=spektro");
  f.print(item.id);
  f.print('\n');

  // === SAVE_AXIS_MODE: WAVELENGTH ===
  f.print("wavelength,Absorbance\n");
  const int len = item.len;
  int lineCounter = 0;
  for (int i = 0; i < len; ++i) {
    const int wavelength = kSpektroFirstNm + i;
    if (wavelength > kSpektroLastNm) break;
    f.print(wavelength);
    f.print(',');
    f.print(item.data[i], 6);
    f.print('\n');
    if ((++lineCounter & 31) == 0) {
      yield();
    }
  }

  yield();
  f.close();
  yield();
  return true;
}

bool writeDegradasiFile(const spektro_ui::DegradasiExport &item,
                        const String &timestamp,
                        const String &jadwalBlock,
                        String &outFilename,
                        String &outError) {
  const char *reaksi = reactionShortName(item.reaction);
  char fnameBuf[64];
  snprintf(fnameBuf, sizeof(fnameBuf), "/%s_%s_%d.csv",
           timestamp.c_str(), reaksi, item.id);
  outFilename = fnameBuf;

  File f = SD.open(fnameBuf, FILE_WRITE);
  if (!f) {
    outError = "open failed: ";
    outError += fnameBuf;
    return false;
  }

  f.print(jadwalBlock);
  f.print('\n');
  f.print("namareaksi=");
  f.print(reaksi);
  f.print(item.id);
  f.print('\n');

  f.print("time,Absorbance\n");
  int lineCounter = 0;
  for (int i = 0; i < item.count; ++i) {
    f.print(item.tMin[i]);
    f.print(',');
    f.print(item.absVal[i], 6);
    f.print('\n');
    if ((++lineCounter & 31) == 0) {
      yield();
    }
  }

  yield();
  f.close();
  yield();
  return true;
}

}  // namespace

namespace sd_storage {

void begin() {
  if (sdSpi == nullptr) {
    sdSpi = new SPIClass(FSPI);
  }
  sdSpi->begin(kSdSckPin, kSdMisoPin, kSdMosiPin, kSdCsPin);

  pinMode(kSdCsPin, OUTPUT);
  digitalWrite(kSdCsPin, HIGH);

  sdBeginAttempted = true;
  sdMounted = SD.begin(kSdCsPin, *sdSpi, kSdSpiFreq);
  if (sdMounted) {
    Serial.println("[sd_storage] mounted ok");
  } else {
    Serial.println("[sd_storage] SD.begin() failed (no card?)");
  }
}

bool isMounted() {
  return sdMounted;
}

void appendBootLog(const String &line) {
  if (!sdMounted) return;
  File f = SD.open("/boot_log.txt", FILE_APPEND);
  if (!f) return;
  f.print(line);
  f.print('\n');
  f.close();
}

bool tryRemount() {
  if (sdMounted) {
    SD.end();
    sdMounted = false;
  }
  if (sdSpi == nullptr) {
    sdSpi = new SPIClass(FSPI);
    sdSpi->begin(kSdSckPin, kSdMisoPin, kSdMosiPin, kSdCsPin);
    pinMode(kSdCsPin, OUTPUT);
    digitalWrite(kSdCsPin, HIGH);
  }
  sdMounted = SD.begin(kSdCsPin, *sdSpi, kSdSpiFreq);
  return sdMounted;
}

SaveResult saveAllSpektroDanDegradasi() {
  SaveResult result;

  if (!sdMounted) {
    if (!tryRemount()) {
      result.status = SaveStatus::NoCard;
      return result;
    }
  }

  spektro_ui::SpektroExport spektroBuf[kMaxSpektroOut];
  spektro_ui::DegradasiExport degradasiBuf[kMaxDegradasiOut];

  const int spektroCount =
      spektro_ui::collectActiveSpektro(spektroBuf, kMaxSpektroOut);
  const int degradasiCount =
      spektro_ui::collectActiveDegradasi(degradasiBuf, kMaxDegradasiOut);

  if (spektroCount == 0 && degradasiCount == 0) {
    result.status = SaveStatus::Empty;
    return result;
  }

  const String timestamp = currentTimestampPrefix();
  const String jadwalBlock = buildJadwalBlock();

  String filename;
  String errorDetail;
  bool anyError = false;

  for (int i = 0; i < spektroCount; ++i) {
    if (writeSpektroFile(spektroBuf[i],
                         timestamp, jadwalBlock,
                         filename, errorDetail)) {
      if (result.fileCount > 0) result.filenames += '\n';
      result.filenames += filename;
      result.fileCount++;
    } else {
      anyError = true;
      if (result.errorDetail.length() > 0) result.errorDetail += "; ";
      result.errorDetail += errorDetail;
    }
    yield();
  }

  for (int i = 0; i < degradasiCount; ++i) {
    if (writeDegradasiFile(degradasiBuf[i],
                           timestamp, jadwalBlock,
                           filename, errorDetail)) {
      if (result.fileCount > 0) result.filenames += '\n';
      result.filenames += filename;
      result.fileCount++;
    } else {
      anyError = true;
      if (result.errorDetail.length() > 0) result.errorDetail += "; ";
      result.errorDetail += errorDetail;
    }
    yield();
  }

  result.status = anyError ? SaveStatus::WriteError : SaveStatus::Ok;
  return result;
}

}  // namespace sd_storage
