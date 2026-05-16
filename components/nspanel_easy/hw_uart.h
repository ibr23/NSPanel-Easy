// hw_uart.h

#pragma once

#ifdef NSPANEL_EASY_HW_UART

#include <cstddef>
#include <cstdint>

/**
 * @file hw_uart.h
 * @brief Constants related to the UART link between the ESP32 and the
 *        Nextion display.
 *
 * The list of Nextion-supported baud rates is centralized here so it can
 * be reused by the boot baud rate scan and any future diagnostic helper
 * without duplicating the values across YAML files.
 */

namespace esphome::nspanel_easy {

/**
 * @brief Nextion-supported baud rates, ordered by likelihood of use in
 *        NSPanel Easy installs.
 *
 * The order is optimized for the boot baud rate scan: the most common
 * rates are tried first. `921600` is the rate used by the official
 * NSPanel Easy TFT files. `115200` and `9600` are the rates used by
 * stock and legacy TFTs. The remaining rates follow the full set of
 * Nextion-supported rates in descending likelihood of use.
 *
 * @note The list is not deduplicated against any caller-provided target
 *       rate. Callers that prepend their own target accept the cost of
 *       probing that rate twice in the worst case.
 */
constexpr uint32_t NEXTION_SUPPORTED_BAUD_RATES[] = {921600, 115200, 9600, 57600,  38400,  19200, 256000,
                                                     512000, 4800,   2400, 230400, 250000, 31250};

constexpr size_t NEXTION_SUPPORTED_BAUD_RATES_COUNT =
    sizeof(NEXTION_SUPPORTED_BAUD_RATES) / sizeof(NEXTION_SUPPORTED_BAUD_RATES[0]);

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_HW_UART
