// nextion_constants.h

#pragma once

#include <cstdint>

#include "all_icons.h"

/**
 * @file nextion_constants.h
 * @brief Display constants for NSPanel interface elements.
 *
 * Defines special icon aliases, RGB565 color constants, and the IconData
 * structure used throughout the NSPanel firmware.
 *
 * All MDI icon constants are auto-generated and live in all_icons.h.
 * This file extends that set with special aliases that are not part of
 * the MDI icon set and are independent of the ZI codepoint mapping.
 *
 * Organization:
 * - Icons:    Special icon aliases (blank, void, unknown, unavailable)
 * - Colors:   RGB565 color values for state visualization
 * - IconData: Structure pairing an icon with a color
 */

namespace esphome::nspanel_easy {

// =============================================================================
// Icon Constants
// =============================================================================

/**
 * @namespace Icons
 * @brief Special icon aliases extending the MDI constants in all_icons.h.
 *
 * These aliases cover display states that have no MDI equivalent:
 * - MDI_NONE, MDI_BLANK, MDI_VOID: suppress icon rendering entirely.
 *   U+FFFF is a guaranteed non-character in Unicode and renders as blank
 *   on the Nextion display regardless of the active font.
 * - MDI_UNKNOWN, MDI_UNAVAILABLE: displayed when an entity state cannot
 *   be determined. Resolved to mdi:alert-circle (MDI_ALERT_CIRCLE) from
 *   all_icons.h so the codepoint stays in sync with the ZI font mapping
 *   automatically when all_icons.h is regenerated.
 *
 * @see all_icons.h for the full set of MDI icon constants.
 */
namespace Icons {
constexpr const char *MDI_NONE = "\uFFFF";             ///< Suppresses icon rendering (U+FFFF, guaranteed non-character)
constexpr const char *MDI_BLANK = MDI_NONE;            ///< Alias for MDI_NONE
constexpr const char *MDI_VOID = MDI_NONE;             ///< Alias for MDI_NONE
constexpr const char *MDI_UNKNOWN = MDI_ALERT_CIRCLE;  ///< Unknown entity state — mdi:alert-circle
constexpr const char *MDI_UNAVAILABLE = MDI_ALERT_CIRCLE;  ///< Unavailable entity state — mdi:alert-circle
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
constexpr uint16_t RGB565_GRAY_DARK = 16904;      ///< Hidden/disabled buttons (RGB565: 0x41C8)
constexpr uint16_t RGB565_GRAY_DARKEST = 6339;    ///< Hidden/disabled buttons (RGB565: 0x18C3)
constexpr uint16_t RGB565_GRAY_LIGHT = 52857;     ///< Gray light/silver (RGB565: 0xCE79)
constexpr uint16_t RGB565_GRAY_MEDIUM = 29614;    ///< Mid gray, secondary text on light bg (RGB565: 0x73AE)
constexpr uint16_t RGB565_GRAY_MOSS = 33808;      ///< Moss gray/green (RGB565: 0x8410)
constexpr uint16_t RGB565_YELLOW_GREEN = 48631;   ///< Inactive buttons (RGB565: 0xBDF7)
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
 * Pairs a pointer to an icon Unicode character with a corresponding
 * RGB565 color value for display on the NSPanel. Used for various
 * entity types including climate, light, cover, etc.
 */
struct IconData {
  const char *icon;  ///< Pointer to icon Unicode character string
  uint16_t color;    ///< RGB565 color value for icon display
};

}  // namespace esphome::nspanel_easy
