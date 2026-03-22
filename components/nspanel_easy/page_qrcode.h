// page_qrcode.h

#pragma once

#ifdef NSPANEL_EASY_PAGE_QRCODE

#include "nextion_components.h"  // For HMIComponent
#include "pages.h"               // For page_names and get_page_id

/**
 * @file page_qrcode.h
 * @brief Nextion component definitions for the QR Code page.
 *
 * This file contains all component constants specific to the QR Code page
 * of the NSPanel interface.
 */

namespace esphome {
namespace nspanel_easy {
namespace hmi {
namespace qrcode {

/**
 * @namespace qrcode
 * @brief Components for the QR Code page.
 *
 * Component ID mapping for the QR Code page
 * Based on the Nextion HMI design file.
 * Note: All components are local scope, so names don't include page prefix.
 */

// Page definition
constexpr HMIComponent PAGE = {"qrcode", 17};  ///< QR Code page (index 17 in page_names array)

// Display components
constexpr HMIComponent QRCODE_LABEL = {"qrcode_label", 1};  ///< QR code label text (100 chars max)

// Button components
constexpr HMIComponent BUTTON_BACK = {"button_back", 2};  ///< Back button (3 chars max) - returns to home

// The QRcode box
constexpr HMIComponent QR = {"qr", 3};  ///< The QRcode itself

// All visual components for iteration
constexpr HMIComponent ALL[] = {PAGE, QRCODE_LABEL, BUTTON_BACK, QR};

constexpr size_t COMPONENT_COUNT = sizeof(ALL) / sizeof(ALL[0]);

}  // namespace qrcode
}  // namespace hmi
}  // namespace nspanel_easy
}  // namespace esphome

#endif  // NSPANEL_EASY_PAGE_QRCODE
