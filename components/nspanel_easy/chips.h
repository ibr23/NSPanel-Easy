// chips.h

#pragma once

#ifdef NSPANEL_EASY_CHIPS

#include <cstdint>
#include <cstring>
#include <string>

/**
 * @file chips.h
 * @brief Chip state shadow for the NSPanel status chip bar.
 *
 * Defines the ChipState struct and the chip_states array that ESPHome uses
 * to remember each chip's last-known icon, color, and visibility so that
 * home and screensaver pages can be re-rendered at any time without waiting
 * for the blueprint to resend data.
 *
 * Chip index mapping (matches CHIP_NAMES order):
 *   0 — chip_relay1
 *   1 — chip_relay2
 *   2 — chip_climate
 *   3 — chip01
 *   4 — chip02
 *   5 — chip03
 *   6 — chip04
 *   7 — chip05
 *   8 — chip06
 *   9 — chip07
 *
 * Component names are unscoped (no page prefix) so that a single write lands
 * on whichever page (home or screensaver) is currently visible.
 */

namespace esphome::nspanel_easy {

/// @brief Total number of chip slots managed by this module.
static constexpr uint8_t CHIP_COUNT = 10;

/**
 * @brief Shadow state for a single chip.
 *
 * Stores the last icon, color, and visibility pushed by the blueprint or
 * by internal relay/climate logic. Used to re-render chips on page load
 * without requiring a new HA push.
 */
struct ChipState {
  char icon[4];    ///< UTF-8 MDI codepoint (1–3 bytes + null terminator)
  uint16_t color;  ///< RGB565 foreground color
  bool visible;    ///< Whether the chip should be shown
};

/**
 * @brief Unscoped Nextion component names for each chip slot.
 *
 * Index order matches chip_states[]. These names are intentionally unscoped
 * so that Nextion resolves them against the currently visible page, which
 * must be either home or screensaver before any render call is issued.
 */
static constexpr const char *CHIP_NAMES[CHIP_COUNT] = {
    "chip_relay1",   ///< Index 0
    "chip_relay2",   ///< Index 1
    "chip_climate",  ///< Index 2
    "chip01",        ///< Index 3
    "chip02",        ///< Index 4
    "chip03",        ///< Index 5
    "chip04",        ///< Index 6
    "chip05",        ///< Index 7
    "chip06",        ///< Index 8
    "chip07",        ///< Index 9
};

// Named indices for direct access by relay and climate logic.
static constexpr uint8_t CHIP_IDX_RELAY1 = 0;   ///< chip_relay1 index in chip_states
static constexpr uint8_t CHIP_IDX_RELAY2 = 1;   ///< chip_relay2 index in chip_states
static constexpr uint8_t CHIP_IDX_CLIMATE = 2;  ///< chip_climate index in chip_states
static constexpr uint8_t CHIP_IDX_USER = 3;     ///< First user chip (chip01) index in chip_states

/// @brief Shadow state array; one entry per chip slot.
extern ChipState chip_states[CHIP_COUNT];

extern bool is_chips_page;

/**
 * @brief Find the chip index for a given component name.
 *
 * @param name Unscoped Nextion component name (e.g. "chip01").
 * @return Index into chip_states[] / CHIP_NAMES[], or UINT8_MAX if not found.
 */
inline uint8_t find_chip_index(const std::string &name) {
  for (uint8_t i = 0; i < CHIP_COUNT; ++i) {
    if (name == CHIP_NAMES[i]) {
      return i;
    }
  }
  return UINT8_MAX;  // sentinel: not found
}

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_CHIPS
