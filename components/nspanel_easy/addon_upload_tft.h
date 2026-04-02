// addon_upload_tft.h - Complete TFT component state management

#pragma once

#ifdef NSPANEL_EASY_ADDON_UPLOAD_TFT

#include <cstdint>

namespace esphome::nspanel_easy {

// TFT upload state variables (previously YAML globals)
extern uint8_t tft_upload_attempt;
extern bool tft_upload_manual_request;
extern uint32_t tft_upload_first_time_synced_ms;
extern bool tft_upload_result;

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_ADDON_UPLOAD_TFT
