// hw_display.cpp

#ifdef NSPANEL_EASY_HW_DISPLAY

#include "hw_display.h"

namespace esphome {
namespace nspanel_easy {

static const char *TAG_COMPONENT_HW_DISPLAY = "nspanel.component.hw_display";

ThemeMode current_theme = ThemeMode::DARK;  ///< Active display theme

uint8_t brightness_current = 100;
uint8_t display_mode = UINT8_MAX;
uint8_t display_charset = UINT8_MAX;

}  // namespace nspanel_easy
}  // namespace esphome

#endif  // NSPANEL_EASY_HW_DISPLAY
