// weather.cpp

#ifdef NSPANEL_EASY_USE_WEATHER

#include "weather.h"

namespace nspanel_easy {

  SunInfo sun_info = {
    .is_up          = true,   // Safe daytime default before first blueprint update or SNTP sync
    .coord_received = false,  // Coordinates not yet received - time proxy active
  };

  uint8_t weather_condition_index = 0;  // Defaults to fallback until first blueprint update

}  // namespace nspanel_easy

#endif  // NSPANEL_EASY_USE_WEATHER
