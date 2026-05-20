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

TEST(UISettings, GetLogViewerSettings_ReturnsAccessibleRef)
{
    settings::UISettings ui;
    EXPECT_DOUBLE_EQ(ui.getLogViewerSettings().getReloadIntervalSec(), 1.0);
}

TEST(UISettings, GetDebugSlotsSettings_ReturnsAccessibleRef)
{
    settings::UISettings ui;
    const auto           sz = ui.getDebugSlotsSettings().getWindowSize();
    EXPECT_EQ(sz.first, 800);
    EXPECT_EQ(sz.second, 600);
}

TEST(UISettings, GetProfileUISettings_ReturnsAccessibleRef)
{
    settings::UISettings ui;
    const auto           sz = ui.getProfileUISettings().getDialogSize();
    EXPECT_EQ(sz.first, 800);
    EXPECT_EQ(sz.second, 600);
}

TEST(UISettings, ConstGetLogViewerSettings_Works)
{
    const settings::UISettings ui;
    EXPECT_DOUBLE_EQ(ui.getLogViewerSettings().getReloadIntervalSec(), 1.0);
}

TEST(UISettings, ConstGetDebugSlotsSettings_Works)
{
    const settings::UISettings ui;
    EXPECT_EQ(ui.getDebugSlotsSettings().getWindowSize().first, 800);
}

TEST(UISettings, ConstGetProfileUISettings_Works)
{
    const settings::UISettings ui;
    EXPECT_EQ(ui.getProfileUISettings().getDialogSize().first, 800);
}

TEST(UISettings, NotDirtyAfterConstruction)
{
    settings::UISettings ui;
    EXPECT_FALSE(ui.isDirty());
}

TEST(UISettings, KeyIsUiSettings)
{
    settings::UISettings ui;
    EXPECT_EQ(ui.getKey(), "uiSettings");
}

TEST(UISettings, ToJsonContainsSubSettingKeys)
{
    settings::UISettings ui;
    const auto           json = ui.toJson();

    EXPECT_TRUE(json.contains("logViewerSettings"));
    EXPECT_TRUE(json.contains("debugSlotsSettings"));
    EXPECT_TRUE(json.contains("profile_ui_settings"));
}

TEST(UISettings, FromJsonRoundTrip_SubSettingDefaultsPreserved)
{
    settings::UISettings ui1;
    const auto           json = ui1.toJson();

    settings::UISettings ui2;
    settings::UISettings::fromJson(json, ui2);

    EXPECT_DOUBLE_EQ(
        ui2.getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
    EXPECT_EQ(ui2.getDebugSlotsSettings().getWindowSize().first, 800);
    EXPECT_EQ(ui2.getProfileUISettings().getDialogSize().second, 600);
}

TEST(UISettings, FromJsonRoundTrip_NotDirtyAfterLoad)
{
    settings::UISettings ui1;
    const auto           json = ui1.toJson();

    settings::UISettings ui2;
    settings::UISettings::fromJson(json, ui2);

    EXPECT_FALSE(ui2.isDirty());
}
