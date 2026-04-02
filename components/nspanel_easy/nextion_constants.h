// nextion_constants.h

#pragma once

#include <cstdint>

/**
 * @file nextion_constants.h
 * @brief Display icon constants and color definitions for NSPanel interface elements.
 *
 * This file contains compile-time constants for icon Unicode characters and display colors
 * used throughout the NSPanel interface. All constants are defined as constexpr to ensure
 * they are stored in flash memory rather than RAM, optimizing memory usage.
 *
 * Organization:
 * - Icons: Unicode characters for display icons (MDI)
 * - Colors: RGB565 color values for state visualization
 * - IconData: Structure pairing icons with colors
 */

namespace esphome::nspanel_easy {

// =============================================================================
// Icon Constants
// =============================================================================

/**
 * @namespace Icons
 * @brief MDI icon Unicode characters for display visualization.
 *
 * These constants represent Material Design Icons (MDI) used to display
 * different device states and modes on the NSPanel display.
 */
namespace Icons {
// Alarm icons
constexpr const char *MDI_SHIELD_ALERT_OUTLINE = "\uEECC";     ///< mdi:shield-alert-outline - Triggered
constexpr const char *MDI_SHIELD_OFF_OUTLINE = "\uE99B";       ///< mdi:shield-off-outline - Disarmed
constexpr const char *MDI_SHIELD_HOME_OUTLINE = "\uECCA";      ///< mdi:shield-home-outline - Armed home
constexpr const char *MDI_SHIELD_LOCK_OUTLINE = "\uECCB";      ///< mdi:shield-lock-outline - Armed away
constexpr const char *MDI_SHIELD_MOON_OUTLINE = "\uF828";      ///< mdi:shield-moon-outline - Armed night
constexpr const char *MDI_SHIELD_AIRPLANE_OUTLINE = "\uECC6";  ///< mdi:shield-airplane-outline - Armed vacation
constexpr const char *MDI_SHIELD_HALF_FULL = "\uE77F";         ///< mdi:shield-half-full - Armed custom bypass
constexpr const char *MDI_SHIELD_OUTLINE = "\uE498";           ///< mdi:shield-outline - Pending/arming

// Climate icons
constexpr const char *MDI_AUTORENEW = "\uE069";      ///< mdi:autorenew - Auto/heat-cool mode
constexpr const char *MDI_SNOWFLAKE = "\uE716";      ///< mdi:snowflake - Cooling mode
constexpr const char *MDI_FIRE = "\uE237";           ///< mdi:fire - Heating mode
constexpr const char *MDI_FAN = "\uE20F";            ///< mdi:fan - Fan mode
constexpr const char *MDI_WATER_PERCENT = "\uE58D";  ///< mdi:water-percent - Dry/dehumidify mode
constexpr const char *MDI_CALENDAR_SYNC = "\uEE8D";  ///< mdi:calendar-sync - Auto mode (deprecated)
constexpr const char *MDI_REFRESH_AUTO = "\uF8F1";   ///< mdi:refresh-auto - Auto mode
constexpr const char *MDI_THERMOMETER = "\uE50E";    ///< mdi:thermometer - Idle state
constexpr const char *MDI_NONE = "\uFFFF";           ///< Hidden/no icon (blank character)

// System/WiFi icons
constexpr const char *MDI_WIFI = "\uE5A8";            ///< mdi:wifi - WiFi connected
constexpr const char *MDI_WIFI_OFF = "\uE5A9";        ///< mdi:wifi-off - WiFi disconnected
constexpr const char *MDI_API_OFF = "\uF256";         ///< mdi:api-off - API disconnected
constexpr const char *MDI_HOME_ASSISTANT = "\uE7CF";  ///< mdi:home-assistant - Blueprint disconnected
constexpr const char *MDI_RESTART = "\uE708";         ///< mdi:restart - System restart

// QR code icons
constexpr const char *MDI_QRCODE_SCAN = "\uE432";  ///< mdi:qrcode-scan - QRcode default icon on the main page
}  // namespace Icons

// =============================================================================
// Color Constants
// =============================================================================

/**
 * @namespace Colors
 * @brief RGB565 color values for state visualization.
 *
 * Color constants used to indicate different device states.
 * Values are in RGB565 format (16-bit color depth) compatible with
 * the Nextion display hardware.
 */
namespace Colors {
constexpr uint16_t RGB565_BLACK = 0;              ///< Hidden/invisible (RGB565: 0x0000)
constexpr uint16_t RGB565_BLUE = 1055;            ///< Cooling action (RGB565: 0x041F)
constexpr uint16_t RGB565_BLUE_BLUETOOTH = 1279;  ///< Bluetooth blue (RGB565: 0x04FF)
constexpr uint16_t RGB565_BLUE_INDIGO = 10597;    ///< Indigo blue (RGB565: 0x2965)
constexpr uint16_t RGB565_CYAN = 1530;            ///< Fan action (RGB565: 0x05FA)
constexpr uint16_t RGB565_CYAN_BRIGHT = 7519;     ///< Bright cyan (RGB565: 0x1D5F)
constexpr uint16_t RGB565_DEEP_ORANGE = 64164;    ///< Heating action (RGB565: 0xFAA4)
constexpr uint16_t RGB565_GREEN = 19818;          ///< Armed/active state (RGB565: 0x4D6A)
constexpr uint16_t RGB565_GRAY = 35921;           ///< Inactive/off state (RGB565: 0x8C51)
constexpr uint16_t RGB565_GRAY_DARK = 16904;      ///< Hidden/disabled buttons
constexpr uint16_t RGB565_GRAY_DARKEST = 6339;    ///< Hidden/disabled buttons
constexpr uint16_t RGB565_GRAY_LIGHT = 52857;     ///< Gray light/silver (RGB565: 0xCE79)
constexpr uint16_t RGB565_GRAY_MEDIUM = 29614;    ///< Mid gray, secondary text on light bg (RGB565: 0x73AE)
constexpr uint16_t RGB565_GRAY_MOSS = 33808;      ///< Moss gray/green (RGB565: 0x8410)
constexpr uint16_t RGB565_YELLOW_GREEN = 48631;   ///< Inactive buttons
constexpr uint16_t RGB565_ORANGE = 64704;         ///< Drying action (RGB565: 0xFCC0)
constexpr uint16_t RGB565_PURPLE_MEDIUM = 38004;  ///< Medium purple (RGB565: 0x9474)
constexpr uint16_t RGB565_RED = 63488;            ///< Alert/triggered state (RGB565: 0xF800)
constexpr uint16_t RGB565_WHITE = 65535;          ///< White/default (RGB565: 0xFFFF)
constexpr uint16_t RGB565_YELLOW = 65024;         ///< Warning/pending state (RGB565: 0xFE00)
constexpr uint16_t RGB565_YELLOW_GOLDEN = 64992;  ///< Golden yellow (RGB565: 0xFDE0)
}  // namespace Colors

// =============================================================================
// Display Data Structures
// =============================================================================

/**
 * @struct IconData
 * @brief Associates an icon character with its display color.
 *
 * This structure pairs a pointer to an icon Unicode character with
 * a corresponding RGB565 color value for display on the NSPanel.
 * Used for various entity types including climate, light, cover, etc.
 */
struct IconData {
  const char *icon;  ///< Pointer to icon Unicode character string
  uint16_t color;    ///< RGB565 color value for icon display
};

}  // namespace esphome::nspanel_easy
