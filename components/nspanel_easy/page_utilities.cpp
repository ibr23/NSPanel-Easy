// page_utilities.cpp

#ifdef NSPANEL_EASY_PAGE_UTILITIES

#include "nextion_constants.h"
#include "page_utilities.h"
#include <cstdlib>  // For malloc/free
#ifdef USE_ESP_IDF
#include "esp_heap_caps.h"
#elif defined(USE_ARDUINO)
#include "esp32-hal-psram.h"
#endif

namespace esphome::nspanel_easy {

bool page_utilities_enabled = false;
uint16_t page_utilities_icon_color = Colors::RGB565_GRAY_LIGHT;

static constexpr size_t UTILITIES_GROUPS_COUNT = 8;

std::vector<UtilitiesGroupValues, esphome::ExternalRAMAllocator<UtilitiesGroupValues>> UtilitiesGroups;

void resetUtilitiesGroups() {
  static constexpr UtilitiesGroupValues INITIAL_UTILITIES_GROUPS[8] = {
      {"grid", "", "", 0},    {"group01", "", "", 0}, {"group02", "", "", 0}, {"group03", "", "", 0},
      {"group04", "", "", 0}, {"group05", "", "", 0}, {"group06", "", "", 0}, {"home", "", "", 0}};

  UtilitiesGroups.assign(INITIAL_UTILITIES_GROUPS, INITIAL_UTILITIES_GROUPS + 8);
}

uint8_t findUtilitiesGroupIndex(const char *group_id) {
  if (UtilitiesGroups.empty())
    return UINT8_MAX;

  int low = 0;
  int high = UtilitiesGroups.size() - 1;

  while (low <= high) {
    int mid = low + (high - low) / 2;
    int cmp = std::strcmp(UtilitiesGroups[mid].group_id, group_id);

    if (cmp < 0) {
      low = mid + 1;
    } else if (cmp > 0) {
      high = mid - 1;
    } else {
      return static_cast<uint8_t>(mid);  // Found
    }
  }

  return UINT8_MAX;  // Not found
}

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_PAGE_UTILITIES
