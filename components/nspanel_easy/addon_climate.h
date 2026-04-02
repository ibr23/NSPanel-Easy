// addon_climate.h

#pragma once

#ifdef NSPANEL_EASY_ADDON_CLIMATE_BASE

#include <string>
#include "nextion_constants.h"

namespace esphome::nspanel_easy {

// Global var for the friendly name of the embedded climate entity
extern std::string addon_climate_friendly_name;

extern bool is_addon_climate_visible;

// =============================================================================
// Climate Enumerations
// =============================================================================

/**
 * @enum ClimateAction
 * @brief Current action of the climate device.
 *
 * Represents what the climate device is currently doing.
 * Values match ESPHome's climate component definitions.
 */
enum ClimateAction : uint8_t {
  CLIMATE_ACTION_OFF = 0,      ///< Climate device is off (inactive or no power)
  CLIMATE_ACTION_COOLING = 2,  ///< Climate device is actively cooling
  CLIMATE_ACTION_HEATING = 3,  ///< Climate device is actively heating
  CLIMATE_ACTION_IDLE = 4,     ///< Climate device is idle (monitoring but no action needed)
  CLIMATE_ACTION_DRYING = 5,   ///< Climate device is drying
  CLIMATE_ACTION_FAN = 6       ///< Climate device is in fan only mode
};

/**
 * @enum ClimateMode
 * @brief Mode the climate device is set to.
 *
 * Represents the operating mode selected for the climate device.
 * Values match ESPHome's climate component definitions.
 */
enum ClimateMode : uint8_t {
  CLIMATE_MODE_OFF = 0,        ///< Climate device is set to off
  CLIMATE_MODE_HEAT_COOL = 1,  ///< Climate device is set to auto (heat/cool)
  CLIMATE_MODE_COOL = 2,       ///< Climate device is set to cool only
  CLIMATE_MODE_HEAT = 3,       ///< Climate device is set to heat only
  CLIMATE_MODE_FAN_ONLY = 4,   ///< Climate device is set to fan only
  CLIMATE_MODE_DRY = 5,        ///< Climate device is set to dry/dehumidify
  CLIMATE_MODE_AUTO = 6        ///< Climate device is set to automatic mode
};

/**
 * @enum ClimateChipVisibility
 * @brief Controls when the climate chip is shown on the home screen.
 *
 * Sent by the blueprint on boot via `action_component_val` with `page: "mem"`
 * and stored in the `climate_chip_visibility` global.
 */
enum ClimateChipVisibility : uint8_t {
  CLIMATE_CHIP_ACTIVE_ONLY = 0,  ///< Show chip only when actively heating/cooling/drying/fan
  CLIMATE_CHIP_ALWAYS = 1,       ///< Always show chip when a climate entity is configured
  CLIMATE_CHIP_NEVER = 2         ///< Never show the chip, regardless of climate state
};

// =============================================================================
// Climate Icon Lookup Tables
// =============================================================================

/**
 * @brief Lookup table for CLIMATE_ACTION_OFF with different modes.
 *
 * Array indices correspond to ClimateMode enum values (0-6):
 * - [0] CLIMATE_MODE_OFF
 * - [1] CLIMATE_MODE_HEAT_COOL
 * - [2] CLIMATE_MODE_COOL
 * - [3] CLIMATE_MODE_HEAT
 * - [4] CLIMATE_MODE_FAN_ONLY
 * - [5] CLIMATE_MODE_DRY
 * - [6] CLIMATE_MODE_AUTO
 *
 * When the climate device is OFF, the icon reflects the selected mode
 * but uses grey color to indicate inactive state.
 */
constexpr IconData climate_off_mode_icons[] = {
    {Icons::MDI_NONE, Colors::RGB565_BLACK},          // CLIMATE_MODE_OFF (0)
    {Icons::MDI_AUTORENEW, Colors::RGB565_GRAY},      // CLIMATE_MODE_HEAT_COOL (1)
    {Icons::MDI_SNOWFLAKE, Colors::RGB565_GRAY},      // CLIMATE_MODE_COOL (2)
    {Icons::MDI_FIRE, Colors::RGB565_GRAY},           // CLIMATE_MODE_HEAT (3)
    {Icons::MDI_FAN, Colors::RGB565_GRAY},            // CLIMATE_MODE_FAN_ONLY (4)
    {Icons::MDI_WATER_PERCENT, Colors::RGB565_GRAY},  // CLIMATE_MODE_DRY (5)
    {Icons::MDI_CALENDAR_SYNC, Colors::RGB565_GRAY}   // CLIMATE_MODE_AUTO (6)
};

/**
 * @brief Lookup table for active climate action states.
 *
 * Array indices correspond to ClimateAction enum values:
 * - [0-1] Unused (reserved)
 * - [2] CLIMATE_ACTION_COOLING
 * - [3] CLIMATE_ACTION_HEATING
 * - [4] CLIMATE_ACTION_IDLE
 * - [5] CLIMATE_ACTION_DRYING
 * - [6] CLIMATE_ACTION_FAN
 *
 * When the climate device is actively operating, the icon and color
 * reflect the current action being performed.
 *
 * @note Indices 0-1 are unused to maintain alignment with the
 *       ClimateAction enum values.
 */
constexpr IconData climate_action_icons[] = {
    {Icons::MDI_NONE, Colors::RGB565_BLACK},            // Unused index 0
    {Icons::MDI_NONE, Colors::RGB565_BLACK},            // Unused index 1
    {Icons::MDI_SNOWFLAKE, Colors::RGB565_BLUE},        // CLIMATE_ACTION_COOLING (2)
    {Icons::MDI_FIRE, Colors::RGB565_DEEP_ORANGE},      // CLIMATE_ACTION_HEATING (3)
    {Icons::MDI_THERMOMETER, Colors::RGB565_GRAY},      // CLIMATE_ACTION_IDLE (4)
    {Icons::MDI_WATER_PERCENT, Colors::RGB565_ORANGE},  // CLIMATE_ACTION_DRYING (5)
    {Icons::MDI_FAN, Colors::RGB565_CYAN}               // CLIMATE_ACTION_FAN (6)
};

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_ADDON_CLIMATE_BASE
