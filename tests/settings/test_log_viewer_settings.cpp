// tests/settings/test_log_viewer_settings.cpp
//
// GoogleTest-based tests for settings::LogViewerSettings,
// settings::DebugSlotsSettings, and settings::ProfileUISettings.
//
// Coverage (LogViewerSettings):
//  - Default values: reload interval, auto-reload, line-wrap, dialog size,
//    max block count
//  - getReloadIntervalMs converts correctly (1.0 s -> 1000 ms)
//  - isDirty is false after construction
//  - getKey returns the schema key
//  - toJson contains expected parameter keys
//  - toJson / fromJson round-trip
//  - LogViewerSettingsException message prefix
//
// Coverage (DebugSlotsSettings):
//  - Default window size 800x600
//  - isDirty is false after construction
//
// Coverage (ProfileUISettings):
//  - Default dialog size 800x600
//  - isDirty is false after construction

#include <gtest/gtest.h>

#include <utility>

#include "settings/debug_slots_settings.hpp"
#include "settings/log_viewer_settings.hpp"
#include "settings/profile_ui_settings.hpp"

// ---------------------------------------------------------------------------
// LogViewerSettings
// ---------------------------------------------------------------------------

TEST(LogViewerSettings, DefaultReloadIntervalSecIs1)
{
    settings::LogViewerSettings lvs;
    EXPECT_DOUBLE_EQ(lvs.getReloadIntervalSec(), 1.0);
}

TEST(LogViewerSettings, DefaultAutoReloadIsFalse)
{
    settings::LogViewerSettings lvs;
    EXPECT_FALSE(lvs.isAutoReloadEnabled());
}

TEST(LogViewerSettings, DefaultLineWrapIsFalse)
{
    settings::LogViewerSettings lvs;
    EXPECT_FALSE(lvs.isLineWrapEnabled());
}

TEST(LogViewerSettings, DefaultDialogSizeIs800x600)
{
    settings::LogViewerSettings   lvs;
    const std::pair<int, int> sz = lvs.getDialogSize();
    EXPECT_EQ(sz.first, 800);
    EXPECT_EQ(sz.second, 600);
}

TEST(LogViewerSettings, DefaultMaxBlockCountIs50000)
{
    settings::LogViewerSettings lvs;
    EXPECT_EQ(lvs.getMaxBlockCount(), 50000);
}

TEST(LogViewerSettings, ReloadIntervalMsIs1000)
{
    settings::LogViewerSettings lvs;
    EXPECT_EQ(lvs.getReloadIntervalMs(), 1000);
}

TEST(LogViewerSettings, NotDirtyAfterConstruction)
{
    settings::LogViewerSettings lvs;
    EXPECT_FALSE(lvs.isDirty());
}

TEST(LogViewerSettings, KeyIsLogViewerSettings)
{
    settings::LogViewerSettings lvs;
    EXPECT_EQ(lvs.getKey(), "logViewerSettings");
}

TEST(LogViewerSettings, ToJsonContainsAllParameterKeys)
{
    settings::LogViewerSettings lvs;
    const auto                  json = lvs.toJson();

    EXPECT_TRUE(json.contains("reloadIntervalSec"));
    EXPECT_TRUE(json.contains("autoReload"));
    EXPECT_TRUE(json.contains("lineWrap"));
    EXPECT_TRUE(json.contains("dialogSize"));
    EXPECT_TRUE(json.contains("maxBlockCount"));
}

TEST(LogViewerSettings, FromJsonRoundTrip_KeysPreserved)
{
    settings::LogViewerSettings lvs1;
    const auto                  json = lvs1.toJson();

    settings::LogViewerSettings lvs2;
    settings::LogViewerSettings::fromJson(json, lvs2);

    EXPECT_DOUBLE_EQ(lvs2.getReloadIntervalSec(), 1.0);
    EXPECT_FALSE(lvs2.isAutoReloadEnabled());
    EXPECT_FALSE(lvs2.isLineWrapEnabled());
    EXPECT_EQ(lvs2.getDialogSize().first, 800);
    EXPECT_EQ(lvs2.getDialogSize().second, 600);
    EXPECT_EQ(lvs2.getMaxBlockCount(), 50000);
}

TEST(LogViewerSettings, FromJsonRoundTrip_NotDirtyAfterLoad)
{
    settings::LogViewerSettings lvs1;
    const auto                  json = lvs1.toJson();

    settings::LogViewerSettings lvs2;
    settings::LogViewerSettings::fromJson(json, lvs2);

    EXPECT_FALSE(lvs2.isDirty());
}

TEST(LogViewerSettingsException, MessageContainsPrefix)
{
    const settings::LogViewerSettingsException ex("viewer error");
    const std::string                          msg = ex.what();
    EXPECT_NE(msg.find("LogViewerSettingsException"), std::string::npos);
    EXPECT_NE(msg.find("viewer error"), std::string::npos);
}

// ---------------------------------------------------------------------------
// DebugSlotsSettings
// ---------------------------------------------------------------------------

TEST(DebugSlotsSettings, DefaultWindowSizeIs800x600)
{
    settings::DebugSlotsSettings dss;
    const auto                   sz = dss.getWindowSize();
    EXPECT_EQ(sz.first, 800);
    EXPECT_EQ(sz.second, 600);
}

TEST(DebugSlotsSettings, NotDirtyAfterConstruction)
{
    settings::DebugSlotsSettings dss;
    EXPECT_FALSE(dss.isDirty());
}

TEST(DebugSlotsSettings, KeyIsDebugSlotsSettings)
{
    settings::DebugSlotsSettings dss;
    EXPECT_EQ(dss.getKey(), "debugSlotsSettings");
}

TEST(DebugSlotsSettings, ToJsonContainsWindowSizeKey)
{
    settings::DebugSlotsSettings dss;
    const auto                   json = dss.toJson();
    EXPECT_TRUE(json.contains("windowSize"));
}

TEST(DebugSlotsSettings, FromJsonRoundTrip_WindowSizePreserved)
{
    settings::DebugSlotsSettings dss1;
    const auto                   json = dss1.toJson();

    settings::DebugSlotsSettings dss2;
    settings::DebugSlotsSettings::fromJson(json, dss2);

    EXPECT_EQ(dss2.getWindowSize().first, 800);
    EXPECT_EQ(dss2.getWindowSize().second, 600);
}

// ---------------------------------------------------------------------------
// ProfileUISettings
// ---------------------------------------------------------------------------

TEST(ProfileUISettings, DefaultDialogSizeIs800x600)
{
    settings::ProfileUISettings pus;
    const auto                  sz = pus.getDialogSize();
    EXPECT_EQ(sz.first, 800);
    EXPECT_EQ(sz.second, 600);
}

TEST(ProfileUISettings, NotDirtyAfterConstruction)
{
    settings::ProfileUISettings pus;
    EXPECT_FALSE(pus.isDirty());
}

TEST(ProfileUISettings, KeyIsProfileUiSettings)
{
    settings::ProfileUISettings pus;
    EXPECT_EQ(pus.getKey(), "profile_ui_settings");
}

TEST(ProfileUISettings, ToJsonContainsDialogSizeKey)
{
    settings::ProfileUISettings pus;
    const auto                  json = pus.toJson();
    EXPECT_TRUE(json.contains("dialogSize"));
}

TEST(ProfileUISettings, FromJsonRoundTrip_DialogSizePreserved)
{
    settings::ProfileUISettings pus1;
    const auto                  json = pus1.toJson();

    settings::ProfileUISettings pus2;
    settings::ProfileUISettings::fromJson(json, pus2);

    EXPECT_EQ(pus2.getDialogSize().first, 800);
    EXPECT_EQ(pus2.getDialogSize().second, 600);
}
