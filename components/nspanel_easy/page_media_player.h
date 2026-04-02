// page_media_player.h

#pragma once

#ifdef NSPANEL_EASY_PAGE_MEDIA_PLAYER

#include <cstdint>

namespace esphome::nspanel_easy {

extern uint8_t last_volume_level;     // Last volume level from Home Assistant
extern uint32_t last_media_duration;  // Last duration from Home Assistant
extern uint32_t last_media_position;  // Last position from Home Assistant

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_PAGE_MEDIA_PLAYER
