#include "app_shared.h"

#include <math.h>

namespace {

constexpr float kFloatEpsilon = 0.001f;

}  // namespace

namespace app {

const char *reactionToJsonName(ReactionType reaction) {
  switch (reaction) {
    case ReactionType::ElectroFenton:    return "ef";
    case ReactionType::Adsorpsi:         return "adsorpsi";
    case ReactionType::Spektrofotometri: return "spektro";
    default:                             return "";
  }
}

const char *reactionToDisplayName(ReactionType reaction) {
  switch (reaction) {
    case ReactionType::ElectroFenton:    return "ELEKTRO FENTON";
    case ReactionType::Adsorpsi:         return "ADSORPSI";
    case ReactionType::Spektrofotometri: return "SPEKTROFOTOMETRI";
    default:                             return "";
  }
}

ReactionType reactionFromString(String value) {
  value.trim();
  value.toLowerCase();

  if (value == "ef")       return ReactionType::ElectroFenton;
  if (value == "adsorpsi") return ReactionType::Adsorpsi;
  if (value == "spektro")  return ReactionType::Spektrofotometri;

  return ReactionType::None;
}

String formatFloatValue(float value) {
  const long rounded = lroundf(value);
  if (fabsf(value - static_cast<float>(rounded)) < kFloatEpsilon) {
    return String(rounded);
  }

  String text = String(value, 2);
  while (text.endsWith("0")) {
    text.remove(text.length() - 1);
  }
  if (text.endsWith(".")) {
    text.remove(text.length() - 1);
  }
  return text;
}

String formatDurationHhMmSs(long totalSeconds) {
  if (totalSeconds < 0) {
    totalSeconds = 0;
  }

  const long hours = totalSeconds / 3600L;
  const long minutes = (totalSeconds % 3600L) / 60L;
  const long seconds = totalSeconds % 60L;

  char buffer[16];
  snprintf(buffer, sizeof(buffer), "%02ld:%02ld:%02ld", hours, minutes, seconds);
  return String(buffer);
}

}  // namespace app
