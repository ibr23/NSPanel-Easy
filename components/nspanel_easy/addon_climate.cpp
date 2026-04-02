// addon_climate.cpp

#ifdef NSPANEL_EASY_ADDON_CLIMATE_BASE

#include "addon_climate.h"

namespace esphome::nspanel_easy {

// Global var for the friendly name of the embedded climate entity
std::string addon_climate_friendly_name = "Thermostat";
bool is_addon_climate_visible = false;

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_ADDON_CLIMATE_BASE
