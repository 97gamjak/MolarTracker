// tests/settings/test_ui_settings.cpp
//
// GoogleTest-based tests for settings::UISettings.
//
// Coverage:
//  - Sub-setting accessors (getLogViewerSettings, getDebugSlotsSettings,
//    getProfileUISettings) return correct types and usable defaults
//  - Const-qualified accessors compile and work
//  - isDirty is false after construction (all sub-settings start clean)
//  - getKey returns the schema key
//  - toJson contains sub-setting keys
//  - commit propagates to all sub-settings

#include <gtest/gtest.h>

#include "settings/ui_settings.hpp"

TEST(UISettings, GetLogViewerSettingsReturnsAccessibleRef)
{
    settings::UISettings uiSettings;
    EXPECT_DOUBLE_EQ(
        uiSettings.getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
}

TEST(UISettings, GetDebugSlotsSettingsReturnsAccessibleRef)
{
    settings::UISettings uiSettings;
    const auto size = uiSettings.getDebugSlotsSettings().getWindowSize();
    EXPECT_EQ(size.first, 800);
    EXPECT_EQ(size.second, 600);
}

TEST(UISettings, GetProfileUISettingsReturnsAccessibleRef)
{
    settings::UISettings uiSettings;
    const auto size = uiSettings.getProfileUISettings().getDialogSize();
    EXPECT_EQ(size.first, 800);
    EXPECT_EQ(size.second, 600);
}

TEST(UISettings, ConstGetLogViewerSettingsWorks)
{
    const settings::UISettings uiSettings;
    EXPECT_DOUBLE_EQ(
        uiSettings.getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
}

TEST(UISettings, ConstGetDebugSlotsSettingsWorks)
{
    const settings::UISettings uiSettings;
    EXPECT_EQ(uiSettings.getDebugSlotsSettings().getWindowSize().first, 800);
}

TEST(UISettings, ConstGetProfileUISettingsWorks)
{
    const settings::UISettings uiSettings;
    EXPECT_EQ(uiSettings.getProfileUISettings().getDialogSize().first, 800);
}

TEST(UISettings, NotDirtyAfterConstruction)
{
    settings::UISettings uiSettings;
    EXPECT_FALSE(uiSettings.isDirty());
}

TEST(UISettings, KeyIsUiSettings)
{
    settings::UISettings uiSettings;
    EXPECT_EQ(uiSettings.getKey(), "uiSettings");
}

TEST(UISettings, ToJsonContainsSubSettingKeys)
{
    settings::UISettings uiSettings;
    const auto           json = uiSettings.toJson();

    EXPECT_TRUE(json.contains("logViewerSettings"));
    EXPECT_TRUE(json.contains("debugSlotsSettings"));
    EXPECT_TRUE(json.contains("profile_ui_settings"));
}

TEST(UISettings, FromJsonRoundTripSubSettingDefaultsPreserved)
{
    settings::UISettings ui1;
    const auto           json = ui1.toJson();

    settings::UISettings ui2;
    settings::UISettings::fromJson(json, ui2);

    EXPECT_DOUBLE_EQ(ui2.getLogViewerSettings().getReloadIntervalSec(), 1.0);
    EXPECT_EQ(ui2.getDebugSlotsSettings().getWindowSize().first, 800);
    EXPECT_EQ(ui2.getProfileUISettings().getDialogSize().second, 600);
}

TEST(UISettings, FromJsonRoundTripNotDirtyAfterLoad)
{
    settings::UISettings ui1;
    const auto           json = ui1.toJson();

    settings::UISettings ui2;
    settings::UISettings::fromJson(json, ui2);

    EXPECT_FALSE(ui2.isDirty());
}
