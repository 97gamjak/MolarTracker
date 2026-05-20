// tests/settings/test_settings.cpp
//
// GoogleTest-based integration tests for settings::Settings.
//
// Coverage:
//  - Construction with a temp directory (no pre-existing file): no throw,
//    all sub-settings accessible with correct defaults
//  - isDirty is true after construction (GeneralSettings._version is set
//    but not yet committed)
//  - getKey returns the schema key
//  - save() writes a settings.json file to the config directory
//  - isDirty is false after save()
//  - save() fires the OnSaved callback
//  - Save then reload: modified defaultProfile is persisted and reloaded
//  - isDirty is true after modifying a sub-setting, false after save

#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <random>
#include <string>

#include "config/logging_base.hpp"
#include "settings/settings.hpp"

namespace
{
    std::filesystem::path unique_temp_settings_dir()
    {
        const auto tmp = std::filesystem::temp_directory_path();

        std::random_device                           rd;
        std::mt19937_64                              gen(rd());
        std::uniform_int_distribution<std::uint64_t> dis;

        return tmp / ("mt_settings_test_" + std::to_string(dis(gen)));
    }

    struct TempDir
    {
        std::filesystem::path path;

        explicit TempDir(std::filesystem::path p) : path(std::move(p))
        {
            std::filesystem::create_directories(path);
        }

        ~TempDir()
        {
            std::error_code ec;
            std::filesystem::remove_all(path, ec);
        }

        TempDir(TempDir const&)            = delete;
        TempDir& operator=(TempDir const&) = delete;
        TempDir(TempDir&&)                 = delete;
        TempDir& operator=(TempDir&&)      = delete;
    };
}   // namespace

TEST(Settings, ConstructWithTempDir_NoThrow)
{
    const TempDir tmp{unique_temp_settings_dir()};
    EXPECT_NO_THROW((void) settings::Settings(tmp.path));
}

TEST(Settings, ConstructWithTempDir_GeneralSettingsAccessible)
{
    const TempDir        tmp{unique_temp_settings_dir()};
    settings::Settings   s(tmp.path);
    EXPECT_FALSE(s.getGeneralSettings().hasDefaultProfile());
}

TEST(Settings, ConstructWithTempDir_LoggingSettingsAccessible)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    EXPECT_EQ(s.getLoggingSettings().getLogDirectory(), "logs");
}

TEST(Settings, ConstructWithTempDir_UISettingsAccessible)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    EXPECT_DOUBLE_EQ(
        s.getUISettings().getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
}

TEST(Settings, ConstGetters_Work)
{
    const TempDir            tmp{unique_temp_settings_dir()};
    const settings::Settings s(tmp.path);
    EXPECT_FALSE(s.getGeneralSettings().hasDefaultProfile());
    EXPECT_EQ(s.getLoggingSettings().getLogDirectory(), "logs");
    EXPECT_DOUBLE_EQ(
        s.getUISettings().getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
}

TEST(Settings, IsDirtyAfterConstruction)
{
    // GeneralSettings sets _version in its constructor but doesn't commit,
    // so the top-level isDirty() returns true.
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    EXPECT_TRUE(s.isDirty());
}

TEST(Settings, KeyIsSettings)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    EXPECT_EQ(s.getKey(), "settings");
}

TEST(Settings, SaveCreatesSettingsFile)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);

    const auto file = tmp.path / "settings.json";
    ASSERT_FALSE(std::filesystem::exists(file));

    s.save();

    EXPECT_TRUE(std::filesystem::exists(file));
}

TEST(Settings, NotDirtyAfterSave)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    s.save();
    EXPECT_FALSE(s.isDirty());
}

TEST(Settings, SaveFiresOnSavedCallback)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);

    bool called = false;
    auto conn   = s.subscribeToSaved([&called]() { called = true; }, nullptr);

    s.save();
    EXPECT_TRUE(called);
}

TEST(Settings, SubscribeToSaved_CallbackNotFiredBeforeSave)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);

    bool called = false;
    auto conn   = s.subscribeToSaved([&called]() { called = true; }, nullptr);

    EXPECT_FALSE(called);
}

TEST(Settings, IsDirty_TrueAfterSettingDefaultProfile)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    s.save();   // clear initial dirty state
    ASSERT_FALSE(s.isDirty());

    s.getGeneralSettings().setDefaultProfile("Dirty");
    EXPECT_TRUE(s.isDirty());
}

TEST(Settings, IsDirty_FalseAfterSaveWhenDefaultProfileSet)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s(tmp.path);
    s.getGeneralSettings().setDefaultProfile("Persisted");
    s.save();
    EXPECT_FALSE(s.isDirty());
}

TEST(Settings, SaveAndReload_DefaultProfilePreserved)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s1(tmp.path);
    s1.getGeneralSettings().setDefaultProfile("Persisted");
    s1.save();

    settings::Settings s2(tmp.path);
    ASSERT_TRUE(s2.getGeneralSettings().hasDefaultProfile());
    EXPECT_EQ(s2.getGeneralSettings().getDefaultProfile().value(), "Persisted");
}

TEST(Settings, SaveAndReload_NotDirtyAfterReload)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s1(tmp.path);
    s1.getGeneralSettings().setDefaultProfile("Reload");
    s1.save();

    settings::Settings s2(tmp.path);
    EXPECT_FALSE(s2.isDirty());
}

TEST(Settings, SaveAndReload_LogLevelPreserved)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings s1(tmp.path);
    const auto result =
        s1.getLoggingSettings().getDefaultLogLevelParam().set(LogLevel::Debug);
    ASSERT_TRUE(result.has_value());
    s1.save();

    settings::Settings s2(tmp.path);
    EXPECT_EQ(
        s2.getLoggingSettings().getDefaultLogLevelParam().get(),
        LogLevel::Debug
    );
}
