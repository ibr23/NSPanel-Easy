// page_notification.h

#pragma once

#ifdef NSPANEL_EASY_PAGE_NOTIFICATION

#include <cstdint>
#include <string>

namespace esphome::nspanel_easy {

extern std::string notification_label;
extern std::string notification_text;
extern uint16_t notification_icon_color_normal;
extern uint16_t notification_icon_color_unread;

}  // namespace esphome::nspanel_easy

#endif  // NSPANEL_EASY_PAGE_NOTIFICATION
