/**
 * Unit tests for components/nspanel_easy/base.cpp/h
 * Tests system flags, blueprint status flags, and utility functions.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <map>

// Mock ESPHome components
namespace esphome {
namespace api {
class CustomAPIDevice {
 public:
  void fire_homeassistant_event(const std::string &event_name, const std::map<std::string, std::string> &data) {
    // Mock implementation
  }
};
}  // namespace api

class App_ {
 public:
  void feed_wdt() {}
};

extern App_ App;

void delay(uint32_t ms) {}
}  // namespace esphome

esphome::App_ esphome::App;

// Define ESP log macros
#define ESP_LOGD(tag, format, ...)
#define ESP_LOGV(tag, format, ...)
#define ESP_LOGVV(tag, format, ...)
#define ESPHOME_LOG_LEVEL 5
#define ESPHOME_LOG_LEVEL_VERBOSE 4

#include "../components/nspanel_easy/base.h"

namespace nspanel_easy {

class BaseTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Reset global flags before each test
    system_flags = SystemFlags();
    blueprint_status_flags = BlueprintStatusFlags();
    cached_device_name = "";
  }

  void TearDown() override {
    // Cleanup after each test
    system_flags = SystemFlags();
    blueprint_status_flags = BlueprintStatusFlags();
    cached_device_name = "";
  }
};

// =============================================================================
// SystemFlags Structure Tests
// =============================================================================

TEST_F(BaseTest, SystemFlags_DefaultConstructor) {
  SystemFlags flags;
  EXPECT_FALSE(flags.wifi_ready);
  EXPECT_FALSE(flags.api_ready);
  EXPECT_FALSE(flags.baud_rate_set);
  EXPECT_FALSE(flags.nextion_ready);
  EXPECT_FALSE(flags.blueprint_ready);
  EXPECT_FALSE(flags.tft_ready);
  EXPECT_FALSE(flags.boot_completed);
  EXPECT_FALSE(flags.version_check_ok);
  EXPECT_FALSE(flags.display_settings_received);
  EXPECT_FALSE(flags.tft_upload_active);
  EXPECT_FALSE(flags.safe_mode_active);
  EXPECT_FALSE(flags.ota_in_progress);
  EXPECT_FALSE(flags.display_sleep);
  EXPECT_EQ(flags.reserved, 0);
}

TEST_F(BaseTest, SystemFlags_SizeOptimization) {
  // Should pack into 2 bytes (uint16_t)
  EXPECT_EQ(sizeof(SystemFlags), sizeof(uint16_t));
}

TEST_F(BaseTest, SystemFlags_IndividualFlagSet) {
  SystemFlags flags;
  flags.wifi_ready = true;
  EXPECT_TRUE(flags.wifi_ready);
  EXPECT_FALSE(flags.api_ready);  // Other flags should remain false
}

TEST_F(BaseTest, SystemFlags_MultipleFlagsSet) {
  SystemFlags flags;
  flags.wifi_ready = true;
  flags.api_ready = true;
  flags.boot_completed = true;

  EXPECT_TRUE(flags.wifi_ready);
  EXPECT_TRUE(flags.api_ready);
  EXPECT_TRUE(flags.boot_completed);
  EXPECT_FALSE(flags.tft_ready);
}

TEST_F(BaseTest, SystemFlags_FlagToggle) {
  SystemFlags flags;
  flags.wifi_ready = true;
  EXPECT_TRUE(flags.wifi_ready);

  flags.wifi_ready = false;
  EXPECT_FALSE(flags.wifi_ready);
}

TEST_F(BaseTest, SystemFlags_AllBootFlagsSet) {
  SystemFlags flags;
  flags.wifi_ready = true;
  flags.api_ready = true;
  flags.baud_rate_set = true;
  flags.nextion_ready = true;
  flags.blueprint_ready = true;
  flags.tft_ready = true;
  flags.boot_completed = true;
  flags.version_check_ok = true;
  flags.display_settings_received = true;

  EXPECT_TRUE(flags.wifi_ready);
  EXPECT_TRUE(flags.api_ready);
  EXPECT_TRUE(flags.baud_rate_set);
  EXPECT_TRUE(flags.nextion_ready);
  EXPECT_TRUE(flags.blueprint_ready);
  EXPECT_TRUE(flags.tft_ready);
  EXPECT_TRUE(flags.boot_completed);
  EXPECT_TRUE(flags.version_check_ok);
  EXPECT_TRUE(flags.display_settings_received);
}

TEST_F(BaseTest, SystemFlags_RuntimeOperationFlags) {
  SystemFlags flags;
  flags.tft_upload_active = true;
  flags.safe_mode_active = true;
  flags.ota_in_progress = true;
  flags.display_sleep = true;

  EXPECT_TRUE(flags.tft_upload_active);
  EXPECT_TRUE(flags.safe_mode_active);
  EXPECT_TRUE(flags.ota_in_progress);
  EXPECT_TRUE(flags.display_sleep);
}

// =============================================================================
// BlueprintStatusFlags Structure Tests
// =============================================================================

TEST_F(BaseTest, BlueprintStatusFlags_DefaultConstructor) {
  BlueprintStatusFlags flags;
  EXPECT_FALSE(flags.page_home);
  EXPECT_FALSE(flags.page_qrcode);
  EXPECT_FALSE(flags.relay_settings);
  EXPECT_FALSE(flags.version);
  EXPECT_FALSE(flags.hw_buttons_settings);
  EXPECT_FALSE(flags.page_utilities);
  EXPECT_EQ(flags.reserved, 0);
}

TEST_F(BaseTest, BlueprintStatusFlags_SizeOptimization) {
  // Should pack into 1 byte (uint8_t)
  EXPECT_EQ(sizeof(BlueprintStatusFlags), sizeof(uint8_t));
}

TEST_F(BaseTest, BlueprintStatusFlags_AllActiveFlagsSetFalse) {
  BlueprintStatusFlags flags;
  EXPECT_FALSE(flags.all_active_flags_set());
}

TEST_F(BaseTest, BlueprintStatusFlags_AllActiveFlagsSetTrue) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  flags.relay_settings = true;
  flags.version = true;
  flags.hw_buttons_settings = true;
  flags.page_utilities = true;

  EXPECT_TRUE(flags.all_active_flags_set());
}

TEST_F(BaseTest, BlueprintStatusFlags_AllActiveFlagsSetPartial) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  flags.relay_settings = true;
  // Missing: version, hw_buttons_settings, page_utilities

  EXPECT_FALSE(flags.all_active_flags_set());
}

TEST_F(BaseTest, BlueprintStatusFlags_CountActiveFlagsSetZero) {
  BlueprintStatusFlags flags;
  EXPECT_EQ(flags.count_active_flags_set(), 0);
}

TEST_F(BaseTest, BlueprintStatusFlags_CountActiveFlagsSetAll) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  flags.relay_settings = true;
  flags.version = true;
  flags.hw_buttons_settings = true;
  flags.page_utilities = true;

  EXPECT_EQ(flags.count_active_flags_set(), 6);
}

TEST_F(BaseTest, BlueprintStatusFlags_CountActiveFlagsSetPartial) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.relay_settings = true;
  flags.version = true;

  EXPECT_EQ(flags.count_active_flags_set(), 3);
}

TEST_F(BaseTest, BlueprintStatusFlags_CompletionPercentageZero) {
  BlueprintStatusFlags flags;
  EXPECT_FLOAT_EQ(flags.get_completion_percentage(), 0.0f);
}

TEST_F(BaseTest, BlueprintStatusFlags_CompletionPercentageFull) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  flags.relay_settings = true;
  flags.version = true;
  flags.hw_buttons_settings = true;
  flags.page_utilities = true;

  EXPECT_FLOAT_EQ(flags.get_completion_percentage(), 100.0f);
}

TEST_F(BaseTest, BlueprintStatusFlags_CompletionPercentageHalf) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  flags.relay_settings = true;
  // 3 out of 6 = 50%

  EXPECT_FLOAT_EQ(flags.get_completion_percentage(), 50.0f);
}

TEST_F(BaseTest, BlueprintStatusFlags_CompletionPercentageThird) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  // 2 out of 6 = 33.333...%

  EXPECT_NEAR(flags.get_completion_percentage(), 33.333f, 0.01f);
}

TEST_F(BaseTest, BlueprintStatusFlags_Reset) {
  BlueprintStatusFlags flags;
  flags.page_home = true;
  flags.page_qrcode = true;
  flags.relay_settings = true;
  flags.version = true;
  flags.hw_buttons_settings = true;
  flags.page_utilities = true;

  flags.reset();

  EXPECT_FALSE(flags.page_home);
  EXPECT_FALSE(flags.page_qrcode);
  EXPECT_FALSE(flags.relay_settings);
  EXPECT_FALSE(flags.version);
  EXPECT_FALSE(flags.hw_buttons_settings);
  EXPECT_FALSE(flags.page_utilities);
  EXPECT_EQ(flags.reserved, 0);
}

TEST_F(BaseTest, BlueprintStatusFlags_ReservedNotCountedInPercentage) {
  BlueprintStatusFlags flags;
  flags.reserved = 3;  // Set reserved bits

  // Reserved bits should not affect percentage
  EXPECT_FLOAT_EQ(flags.get_completion_percentage(), 0.0f);
  EXPECT_EQ(flags.count_active_flags_set(), 0);
  EXPECT_FALSE(flags.all_active_flags_set());
}

// =============================================================================
// Global Variables Tests
// =============================================================================

TEST_F(BaseTest, GlobalSystemFlags_Accessible) {
  system_flags.wifi_ready = true;
  EXPECT_TRUE(system_flags.wifi_ready);
}

TEST_F(BaseTest, GlobalBlueprintStatusFlags_Accessible) {
  blueprint_status_flags.page_home = true;
  EXPECT_TRUE(blueprint_status_flags.page_home);
}

TEST_F(BaseTest, GlobalCachedDeviceName_Accessible) {
  cached_device_name = "test_device";
  EXPECT_EQ(cached_device_name, "test_device");
}

TEST_F(BaseTest, GlobalCachedDeviceName_DefaultEmpty) { EXPECT_TRUE(cached_device_name.empty()); }

// =============================================================================
// is_device_ready_for_tasks() Tests
// =============================================================================

TEST_F(BaseTest, IsDeviceReadyForTasks_BootNotCompleted) {
  system_flags.boot_completed = false;
  EXPECT_FALSE(is_device_ready_for_tasks());
}

TEST_F(BaseTest, IsDeviceReadyForTasks_OtaInProgress) {
  system_flags.boot_completed = true;
  system_flags.ota_in_progress = true;
  EXPECT_FALSE(is_device_ready_for_tasks());
}

TEST_F(BaseTest, IsDeviceReadyForTasks_TftUploadActive) {
  system_flags.boot_completed = true;
  system_flags.tft_upload_active = true;
  EXPECT_FALSE(is_device_ready_for_tasks());
}

TEST_F(BaseTest, IsDeviceReadyForTasks_SafeModeActive) {
  system_flags.boot_completed = true;
  system_flags.safe_mode_active = true;
  EXPECT_FALSE(is_device_ready_for_tasks());
}

TEST_F(BaseTest, IsDeviceReadyForTasks_AllConditionsMet) {
  system_flags.boot_completed = true;
  system_flags.ota_in_progress = false;
  system_flags.tft_upload_active = false;
  system_flags.safe_mode_active = false;
  EXPECT_TRUE(is_device_ready_for_tasks());
}

TEST_F(BaseTest, IsDeviceReadyForTasks_MultipleBlockingOperations) {
  system_flags.boot_completed = true;
  system_flags.ota_in_progress = true;
  system_flags.tft_upload_active = true;
  system_flags.safe_mode_active = true;
  EXPECT_FALSE(is_device_ready_for_tasks());
}

// =============================================================================
// is_blueprint_fully_ready() Tests
// =============================================================================

TEST_F(BaseTest, IsBlueprintFullyReady_NoFlagsSet) { EXPECT_FALSE(is_blueprint_fully_ready()); }

TEST_F(BaseTest, IsBlueprintFullyReady_AllFlagsSet) {
  blueprint_status_flags.page_home = true;
  blueprint_status_flags.page_qrcode = true;
  blueprint_status_flags.relay_settings = true;
  blueprint_status_flags.version = true;
  blueprint_status_flags.hw_buttons_settings = true;
  blueprint_status_flags.page_utilities = true;

  EXPECT_TRUE(is_blueprint_fully_ready());
}

TEST_F(BaseTest, IsBlueprintFullyReady_UpdatesSystemFlag) {
  blueprint_status_flags.page_home = true;
  blueprint_status_flags.page_qrcode = true;
  blueprint_status_flags.relay_settings = true;
  blueprint_status_flags.version = true;
  blueprint_status_flags.hw_buttons_settings = true;
  blueprint_status_flags.page_utilities = true;

  EXPECT_FALSE(system_flags.blueprint_ready);  // Initially false

  is_blueprint_fully_ready();

  EXPECT_TRUE(system_flags.blueprint_ready);  // Updated to true
}

TEST_F(BaseTest, IsBlueprintFullyReady_PartialFlagsSet) {
  blueprint_status_flags.page_home = true;
  blueprint_status_flags.page_qrcode = true;

  EXPECT_FALSE(is_blueprint_fully_ready());
  EXPECT_FALSE(system_flags.blueprint_ready);
}

// =============================================================================
// feed_wdt_delay() Tests
// =============================================================================

TEST_F(BaseTest, FeedWdtDelay_DefaultParameter) { EXPECT_NO_THROW(feed_wdt_delay()); }

TEST_F(BaseTest, FeedWdtDelay_CustomParameter) { EXPECT_NO_THROW(feed_wdt_delay(10)); }

TEST_F(BaseTest, FeedWdtDelay_ZeroDelay) { EXPECT_NO_THROW(feed_wdt_delay(0)); }

TEST_F(BaseTest, FeedWdtDelay_LargeDelay) { EXPECT_NO_THROW(feed_wdt_delay(1000)); }

// =============================================================================
// fire_ha_event() Tests
// =============================================================================

TEST_F(BaseTest, FireHaEvent_SimpleEvent) {
  cached_device_name = "test_panel";
  EXPECT_NO_THROW(fire_ha_event("test_event"));
}

TEST_F(BaseTest, FireHaEvent_WithData) {
  cached_device_name = "test_panel";
  std::map<std::string, std::string> data = {{"key1", "value1"}, {"key2", "value2"}};
  EXPECT_NO_THROW(fire_ha_event("test_event", data));
}

TEST_F(BaseTest, FireHaEvent_EmptyDeviceName) {
  cached_device_name = "";
  EXPECT_NO_THROW(fire_ha_event("test_event"));
}

TEST_F(BaseTest, FireHaEvent_EmptyEventType) {
  cached_device_name = "test_panel";
  EXPECT_NO_THROW(fire_ha_event(""));
}

TEST_F(BaseTest, FireHaEvent_EmptyData) {
  cached_device_name = "test_panel";
  std::map<std::string, std::string> empty_data;
  EXPECT_NO_THROW(fire_ha_event("test_event", empty_data));
}

TEST_F(BaseTest, FireHaEvent_SpecialCharactersInData) {
  cached_device_name = "test_panel";
  std::map<std::string, std::string> data = {{"special", "!@#$%^&*()"}, {"unicode", "héllo wörld"}};
  EXPECT_NO_THROW(fire_ha_event("test_event", data));
}

// =============================================================================
// Edge Cases and Boundary Tests
// =============================================================================

TEST_F(BaseTest, SystemFlags_AllFlagsSetAndUnset) {
  system_flags.wifi_ready = true;
  system_flags.api_ready = true;
  system_flags.baud_rate_set = true;
  system_flags.nextion_ready = true;
  system_flags.blueprint_ready = true;
  system_flags.tft_ready = true;
  system_flags.boot_completed = true;
  system_flags.version_check_ok = true;
  system_flags.display_settings_received = true;
  system_flags.tft_upload_active = true;
  system_flags.safe_mode_active = true;
  system_flags.ota_in_progress = true;
  system_flags.display_sleep = true;

  // All flags should be true
  EXPECT_TRUE(system_flags.wifi_ready);
  EXPECT_TRUE(system_flags.boot_completed);

  // Reset all
  system_flags = SystemFlags();
  EXPECT_FALSE(system_flags.wifi_ready);
  EXPECT_FALSE(system_flags.boot_completed);
}

TEST_F(BaseTest, BlueprintStatusFlags_IncrementalCompletion) {
  EXPECT_FLOAT_EQ(blueprint_status_flags.get_completion_percentage(), 0.0f);

  blueprint_status_flags.page_home = true;
  EXPECT_NEAR(blueprint_status_flags.get_completion_percentage(), 16.667f, 0.01f);

  blueprint_status_flags.page_qrcode = true;
  EXPECT_NEAR(blueprint_status_flags.get_completion_percentage(), 33.333f, 0.01f);

  blueprint_status_flags.relay_settings = true;
  EXPECT_FLOAT_EQ(blueprint_status_flags.get_completion_percentage(), 50.0f);

  blueprint_status_flags.version = true;
  EXPECT_NEAR(blueprint_status_flags.get_completion_percentage(), 66.667f, 0.01f);

  blueprint_status_flags.hw_buttons_settings = true;
  EXPECT_NEAR(blueprint_status_flags.get_completion_percentage(), 83.333f, 0.01f);

  blueprint_status_flags.page_utilities = true;
  EXPECT_FLOAT_EQ(blueprint_status_flags.get_completion_percentage(), 100.0f);
}

TEST_F(BaseTest, CachedDeviceName_LongString) {
  std::string long_name(1000, 'A');
  cached_device_name = long_name;
  EXPECT_EQ(cached_device_name.length(), 1000);
  EXPECT_EQ(cached_device_name, long_name);
}

TEST_F(BaseTest, CachedDeviceName_SpecialCharacters) {
  cached_device_name = "device-name_123!@#";
  EXPECT_EQ(cached_device_name, "device-name_123!@#");
}

// =============================================================================
// Regression Tests
// =============================================================================

TEST_F(BaseTest, Regression_FlagsIndependence) {
  // Setting one flag should not affect others
  system_flags.wifi_ready = true;
  EXPECT_TRUE(system_flags.wifi_ready);
  EXPECT_FALSE(system_flags.api_ready);
  EXPECT_FALSE(system_flags.boot_completed);
}

TEST_F(BaseTest, Regression_BlueprintPercentageAccuracy) {
  // Verify percentage calculation is accurate
  BlueprintStatusFlags flags;
  flags.page_home = true;  // 1/6
  EXPECT_NEAR(flags.get_completion_percentage(), 16.6667f, 0.01f);
}

TEST_F(BaseTest, Regression_NoMemoryCorruption) {
  // Repeatedly set and reset flags
  for (int i = 0; i < 1000; ++i) {
    system_flags.wifi_ready = true;
    system_flags.wifi_ready = false;
    blueprint_status_flags.page_home = true;
    blueprint_status_flags.reset();
  }
  EXPECT_FALSE(system_flags.wifi_ready);
  EXPECT_FALSE(blueprint_status_flags.page_home);
}

}  // namespace nspanel_easy

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}