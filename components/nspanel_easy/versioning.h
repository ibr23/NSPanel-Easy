// versioning.h

#pragma once

#ifdef NSPANEL_EASY_VERSIONING

#include <cstdint>

namespace esphome {
namespace nspanel_easy {

extern uint8_t version_blueprint;  // Blueprint version/revision
extern uint8_t version_tft;        // TFT version/revision

}  // namespace nspanel_easy
}  // namespace esphome

#endif  // NSPANEL_EASY_VERSIONING
