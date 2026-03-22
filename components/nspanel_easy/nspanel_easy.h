#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace nspanel_easy {

/// @brief Main NSPanel Easy component — provides on_setup and on_dump_config triggers.
class NSPanelEasyComponent : public Component {
 public:
  void setup() override { this->on_setup_callbacks_.call(); }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "NSPanel Easy:");
    this->on_dump_config_callbacks_.call();
  }

  void add_on_setup_callback(std::function<void()> &&callback) { this->on_setup_callbacks_.add(std::move(callback)); }

  void add_on_dump_config_callback(std::function<void()> &&callback) {
    this->on_dump_config_callbacks_.add(std::move(callback));
  }

 protected:
  static constexpr const char *TAG = "nspanel_easy";

  CallbackManager<void()> on_setup_callbacks_{};
  CallbackManager<void()> on_dump_config_callbacks_{};
};

/// @brief Trigger fired when the component's setup() completes.
class SetupTrigger : public Trigger<> {
 public:
  explicit SetupTrigger(NSPanelEasyComponent *nspanel_easy) {
    nspanel_easy->add_on_setup_callback([this]() { this->trigger(); });
  }
};

/// @brief Trigger fired when ESPHome calls dump_config() on this component.
class DumpConfigTrigger : public Trigger<> {
 public:
  explicit DumpConfigTrigger(NSPanelEasyComponent *nspanel_easy) {
    nspanel_easy->add_on_dump_config_callback([this]() { this->trigger(); });
  }
};

}  // namespace nspanel_easy
}  // namespace esphome
