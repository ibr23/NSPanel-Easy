// page_climate.h

#pragma once

#ifdef NSPANEL_EASY_PAGE_CLIMATE

#include <cstdint>
#include <string>

namespace esphome::nspanel_easy {

extern float set_climate_current_temp;
extern uint32_t set_climate_supported_features;
extern float set_climate_target_temp;
extern float set_climate_target_temp_high;
extern float set_climate_target_temp_low;
extern uint8_t set_climate_temp_step;
extern uint16_t set_climate_total_steps;
extern uint16_t set_climate_temp_offset;
extern std::string set_climate_climate_icon;
extern bool set_climate_embedded_climate;

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_PAGE_CLIMATE
