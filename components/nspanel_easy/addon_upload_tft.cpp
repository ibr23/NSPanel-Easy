// addon_upload_tft.cpp - TFT component state definitions

#ifdef NSPANEL_EASY_ADDON_UPLOAD_TFT

#include "addon_upload_tft.h"

namespace esphome {
namespace nspanel_easy {

// TFT upload state variables (previously YAML globals)
uint8_t tft_upload_attempt = 0;
bool tft_upload_manual_request = false;
uint32_t tft_upload_first_time_synced_ms = 0;
bool tft_upload_result = false;

}  // namespace nspanel_easy
}  // namespace esphome

#endif  // NSPANEL_EASY_ADDON_UPLOAD_TFT
