/**
 * Unit tests for components/nspanel_easy/addon_climate.cpp/h
 * Tests climate enumerations, icon lookup tables, and global variables.
 */

#include <gtest/gtest.h>
#include <cstdint>
#include <set>
#include <string>

// Define the macro before including the header
#define NSPANEL_EASY_ADDON_CLIMATE_BASE

// Mock icons.h structures
namespace nspanel_easy {
namespace Icons {
constexpr uint16_t NONE = 0;
constexpr uint16_t AUTORENEW = 1;
constexpr uint16_t SNOWFLAKE = 2;
constexpr uint16_t FIRE = 3;
constexpr uint16_t FAN = 4;
constexpr uint16_t WATER_PERCENT = 5;
constexpr uint16_t CALENDAR_SYNC = 6;
constexpr uint16_t THERMOMETER = 7;
}  // namespace Icons

namespace Colors {
constexpr uint32_t BLACK = 0x000000;
constexpr uint32_t GRAY = 0x808080;
constexpr uint32_t BLUE = 0x0000FF;
constexpr uint32_t DEEP_ORANGE = 0xFF5722;
constexpr uint32_t ORANGE = 0xFFA500;
constexpr uint32_t CYAN = 0x00FFFF;
}  // namespace Colors

struct IconData {
  uint16_t icon;
  uint32_t color;
};
}  // namespace nspanel_easy

#include "../components/nspanel_easy/addon_climate.h"

namespace nspanel_easy {

class AddonClimateTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Reset global variables before each test
    addon_climate_friendly_name = "Thermostat";
    is_addon_climate_visible = false;
  }
};

// =============================================================================
// Global Variable Tests
// =============================================================================

TEST_F(AddonClimateTest, FriendlyNameDefaultValue) { EXPECT_EQ(addon_climate_friendly_name, "Thermostat"); }

TEST_F(AddonClimateTest, FriendlyNameCanBeModified) {
  addon_climate_friendly_name = "Living Room Climate";
  EXPECT_EQ(addon_climate_friendly_name, "Living Room Climate");
}

TEST_F(AddonClimateTest, VisibilityDefaultValue) { EXPECT_FALSE(is_addon_climate_visible); }

TEST_F(AddonClimateTest, VisibilityCanBeToggled) {
  is_addon_climate_visible = true;
  EXPECT_TRUE(is_addon_climate_visible);

  is_addon_climate_visible = false;
  EXPECT_FALSE(is_addon_climate_visible);
}

// =============================================================================
// Climate Action Enumeration Tests
// =============================================================================

TEST_F(AddonClimateTest, ClimateActionEnumValues) {
  EXPECT_EQ(CLIMATE_ACTION_OFF, 0);
  EXPECT_EQ(CLIMATE_ACTION_COOLING, 2);
  EXPECT_EQ(CLIMATE_ACTION_HEATING, 3);
  EXPECT_EQ(CLIMATE_ACTION_IDLE, 4);
  EXPECT_EQ(CLIMATE_ACTION_DRYING, 5);
  EXPECT_EQ(CLIMATE_ACTION_FAN, 6);
}

TEST_F(AddonClimateTest, ClimateActionEnumSize) {
  // Verify enum is uint8_t
  EXPECT_EQ(sizeof(ClimateAction), sizeof(uint8_t));
}

TEST_F(AddonClimateTest, ClimateActionValuesAreUnique) {
  // Ensure no duplicate values
  std::set<uint8_t> values = {CLIMATE_ACTION_OFF,  CLIMATE_ACTION_COOLING, CLIMATE_ACTION_HEATING,
                              CLIMATE_ACTION_IDLE, CLIMATE_ACTION_DRYING,  CLIMATE_ACTION_FAN};
  EXPECT_EQ(values.size(), 6);
}

// =============================================================================
// Climate Mode Enumeration Tests
// =============================================================================

TEST_F(AddonClimateTest, ClimateModeEnumValues) {
  EXPECT_EQ(CLIMATE_MODE_OFF, 0);
  EXPECT_EQ(CLIMATE_MODE_HEAT_COOL, 1);
  EXPECT_EQ(CLIMATE_MODE_COOL, 2);
  EXPECT_EQ(CLIMATE_MODE_HEAT, 3);
  EXPECT_EQ(CLIMATE_MODE_FAN_ONLY, 4);
  EXPECT_EQ(CLIMATE_MODE_DRY, 5);
  EXPECT_EQ(CLIMATE_MODE_AUTO, 6);
}

TEST_F(AddonClimateTest, ClimateModeEnumSize) {
  // Verify enum is uint8_t
  EXPECT_EQ(sizeof(ClimateMode), sizeof(uint8_t));
}

TEST_F(AddonClimateTest, ClimateModeValuesAreUnique) {
  std::set<uint8_t> values = {CLIMATE_MODE_OFF,      CLIMATE_MODE_HEAT_COOL, CLIMATE_MODE_COOL, CLIMATE_MODE_HEAT,
                              CLIMATE_MODE_FAN_ONLY, CLIMATE_MODE_DRY,       CLIMATE_MODE_AUTO};
  EXPECT_EQ(values.size(), 7);
}

// =============================================================================
// Climate Off Mode Icon Lookup Table Tests
// =============================================================================

TEST_F(AddonClimateTest, ClimateOffModeIconsTableSize) {
  EXPECT_EQ(sizeof(climate_off_mode_icons) / sizeof(IconData), 7);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeOff) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_OFF];
  EXPECT_EQ(icon_data.icon, Icons::NONE);
  EXPECT_EQ(icon_data.color, Colors::BLACK);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeHeatCool) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_HEAT_COOL];
  EXPECT_EQ(icon_data.icon, Icons::AUTORENEW);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeCool) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_COOL];
  EXPECT_EQ(icon_data.icon, Icons::SNOWFLAKE);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeHeat) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_HEAT];
  EXPECT_EQ(icon_data.icon, Icons::FIRE);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeFanOnly) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_FAN_ONLY];
  EXPECT_EQ(icon_data.icon, Icons::FAN);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeDry) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_DRY];
  EXPECT_EQ(icon_data.icon, Icons::WATER_PERCENT);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_ModeAuto) {
  const auto &icon_data = climate_off_mode_icons[CLIMATE_MODE_AUTO];
  EXPECT_EQ(icon_data.icon, Icons::CALENDAR_SYNC);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateOffModeIcons_AllGrayExceptModeOff) {
  // All icons except MODE_OFF should be gray when device is off
  for (size_t i = 1; i < 7; ++i) {
    EXPECT_EQ(climate_off_mode_icons[i].color, Colors::GRAY) << "Icon at index " << i << " should be GRAY when off";
  }
}

// =============================================================================
// Climate Action Icon Lookup Table Tests
// =============================================================================

TEST_F(AddonClimateTest, ClimateActionIconsTableSize) { EXPECT_EQ(sizeof(climate_action_icons) / sizeof(IconData), 7); }

TEST_F(AddonClimateTest, ClimateActionIcons_Cooling) {
  const auto &icon_data = climate_action_icons[CLIMATE_ACTION_COOLING];
  EXPECT_EQ(icon_data.icon, Icons::SNOWFLAKE);
  EXPECT_EQ(icon_data.color, Colors::BLUE);
}

TEST_F(AddonClimateTest, ClimateActionIcons_Heating) {
  const auto &icon_data = climate_action_icons[CLIMATE_ACTION_HEATING];
  EXPECT_EQ(icon_data.icon, Icons::FIRE);
  EXPECT_EQ(icon_data.color, Colors::DEEP_ORANGE);
}

TEST_F(AddonClimateTest, ClimateActionIcons_Idle) {
  const auto &icon_data = climate_action_icons[CLIMATE_ACTION_IDLE];
  EXPECT_EQ(icon_data.icon, Icons::THERMOMETER);
  EXPECT_EQ(icon_data.color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ClimateActionIcons_Drying) {
  const auto &icon_data = climate_action_icons[CLIMATE_ACTION_DRYING];
  EXPECT_EQ(icon_data.icon, Icons::WATER_PERCENT);
  EXPECT_EQ(icon_data.color, Colors::ORANGE);
}

TEST_F(AddonClimateTest, ClimateActionIcons_Fan) {
  const auto &icon_data = climate_action_icons[CLIMATE_ACTION_FAN];
  EXPECT_EQ(icon_data.icon, Icons::FAN);
  EXPECT_EQ(icon_data.color, Colors::CYAN);
}

TEST_F(AddonClimateTest, ClimateActionIcons_UnusedIndices) {
  // Indices 0 and 1 are unused (reserved)
  EXPECT_EQ(climate_action_icons[0].icon, Icons::NONE);
  EXPECT_EQ(climate_action_icons[0].color, Colors::BLACK);
  EXPECT_EQ(climate_action_icons[1].icon, Icons::NONE);
  EXPECT_EQ(climate_action_icons[1].color, Colors::BLACK);
}

// =============================================================================
// Icon Consistency Tests
// =============================================================================

TEST_F(AddonClimateTest, IconConsistency_SnowflakeUsedForCooling) {
  // Snowflake should be used for both OFF/COOL mode and COOLING action
  EXPECT_EQ(climate_off_mode_icons[CLIMATE_MODE_COOL].icon, Icons::SNOWFLAKE);
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_COOLING].icon, Icons::SNOWFLAKE);
}

TEST_F(AddonClimateTest, IconConsistency_FireUsedForHeating) {
  // Fire should be used for both OFF/HEAT mode and HEATING action
  EXPECT_EQ(climate_off_mode_icons[CLIMATE_MODE_HEAT].icon, Icons::FIRE);
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_HEATING].icon, Icons::FIRE);
}

TEST_F(AddonClimateTest, IconConsistency_FanUsedForFanMode) {
  // Fan should be used for both OFF/FAN_ONLY mode and FAN action
  EXPECT_EQ(climate_off_mode_icons[CLIMATE_MODE_FAN_ONLY].icon, Icons::FAN);
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_FAN].icon, Icons::FAN);
}

TEST_F(AddonClimateTest, IconConsistency_WaterPercentUsedForDry) {
  // Water percent should be used for both OFF/DRY mode and DRYING action
  EXPECT_EQ(climate_off_mode_icons[CLIMATE_MODE_DRY].icon, Icons::WATER_PERCENT);
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_DRYING].icon, Icons::WATER_PERCENT);
}

// =============================================================================
// Color Semantics Tests
// =============================================================================

TEST_F(AddonClimateTest, ColorSemantics_CoolingIsBlue) {
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_COOLING].color, Colors::BLUE);
}

TEST_F(AddonClimateTest, ColorSemantics_HeatingIsDeepOrange) {
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_HEATING].color, Colors::DEEP_ORANGE);
}

TEST_F(AddonClimateTest, ColorSemantics_IdleIsGray) {
  EXPECT_EQ(climate_action_icons[CLIMATE_ACTION_IDLE].color, Colors::GRAY);
}

TEST_F(AddonClimateTest, ColorSemantics_OffModesAreGrayOrBlack) {
  // All off modes should use GRAY or BLACK to indicate inactive state
  for (size_t i = 0; i < 7; ++i) {
    uint32_t color = climate_off_mode_icons[i].color;
    EXPECT_TRUE(color == Colors::GRAY || color == Colors::BLACK)
        << "Off mode at index " << i << " should be GRAY or BLACK";
  }
}

// =============================================================================
// Boundary and Edge Case Tests
// =============================================================================

TEST_F(AddonClimateTest, LookupTable_ValidIndicesAccess) {
  // Test that all valid mode indices can be accessed
  for (int i = 0; i < 7; ++i) {
    EXPECT_NO_THROW({
      auto icon = climate_off_mode_icons[i];
      (void) icon;  // Suppress unused variable warning
    });
  }
}

TEST_F(AddonClimateTest, LookupTable_ValidActionIndicesAccess) {
  // Test that all valid action indices can be accessed
  for (int i = 0; i < 7; ++i) {
    EXPECT_NO_THROW({
      auto icon = climate_action_icons[i];
      (void) icon;  // Suppress unused variable warning
    });
  }
}

TEST_F(AddonClimateTest, StringOperations_FriendlyNameEmpty) {
  addon_climate_friendly_name = "";
  EXPECT_TRUE(addon_climate_friendly_name.empty());
}

TEST_F(AddonClimateTest, StringOperations_FriendlyNameLong) {
  std::string long_name(1000, 'A');
  addon_climate_friendly_name = long_name;
  EXPECT_EQ(addon_climate_friendly_name.length(), 1000);
}

TEST_F(AddonClimateTest, StringOperations_FriendlyNameSpecialCharacters) {
  addon_climate_friendly_name = "Thërmöstàt 123 !@#";
  EXPECT_EQ(addon_climate_friendly_name, "Thërmöstàt 123 !@#");
}

// =============================================================================
// Regression Tests
// =============================================================================

TEST_F(AddonClimateTest, Regression_NoIconDuplicatesInOffModes) {
  // Verify each mode has a unique icon (except MODE_OFF which uses NONE)
  std::set<uint16_t> icons;
  for (size_t i = 1; i < 7; ++i) {  // Skip index 0 (MODE_OFF)
    icons.insert(climate_off_mode_icons[i].icon);
  }
  EXPECT_EQ(icons.size(), 6) << "Each mode should have a unique icon";
}

TEST_F(AddonClimateTest, Regression_NoIconDuplicatesInActions) {
  // Verify each action has a unique icon (except reserved indices)
  std::set<uint16_t> icons;
  for (size_t i = 2; i < 7; ++i) {  // Skip indices 0-1 (reserved)
    icons.insert(climate_action_icons[i].icon);
  }
  EXPECT_EQ(icons.size(), 5) << "Each action should have a unique icon";
}

}  // namespace nspanel_easy

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}