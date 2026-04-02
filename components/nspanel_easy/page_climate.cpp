// page_climate.cpp

#ifdef NSPANEL_EASY_PAGE_CLIMATE

#include "page_climate.h"

namespace esphome::nspanel_easy {

float set_climate_current_temp;
uint32_t set_climate_supported_features;
float set_climate_target_temp;
float set_climate_target_temp_high;
float set_climate_target_temp_low;
uint8_t set_climate_temp_step;
uint16_t set_climate_total_steps;
uint16_t set_climate_temp_offset;
std::string set_climate_climate_icon;
bool set_climate_embedded_climate;

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_PAGE_CLIMATE
