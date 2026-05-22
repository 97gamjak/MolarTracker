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

        std::random_device                           random;
        std::mt19937_64                              gen(random());
        std::uniform_int_distribution<std::uint64_t> dis;

        return tmp / ("mt_settings_test_" + std::to_string(dis(gen)));
    }

    struct TempDir
    {
        std::filesystem::path path;

        explicit TempDir(std::filesystem::path path_) : path(std::move(path_))
        {
            std::filesystem::create_directories(path);
        }

        ~TempDir()
        {
            std::error_code errorCode;
            std::filesystem::remove_all(path, errorCode);
        }

        TempDir(TempDir const&)            = delete;
        TempDir& operator=(TempDir const&) = delete;
        TempDir(TempDir&&)                 = delete;
        TempDir& operator=(TempDir&&)      = delete;
    };
}   // namespace

TEST(Settings, ConstructWithTempDirNoThrow)
{
    const TempDir tmp{unique_temp_settings_dir()};
    EXPECT_NO_THROW((void) settings::Settings(tmp.path));
}

TEST(Settings, ConstructWithTempDirGeneralSettingsAccessible)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    EXPECT_FALSE(settings.getGeneralSettings().hasDefaultProfile());
}

TEST(Settings, ConstructWithTempDirLoggingSettingsAccessible)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    EXPECT_EQ(settings.getLoggingSettings().getLogDirectory(), "logs");
}

TEST(Settings, ConstructWithTempDirUISettingsAccessible)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    EXPECT_DOUBLE_EQ(
        settings.getUISettings().getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
}

TEST(Settings, ConstGettersWork)
{
    const TempDir            tmp{unique_temp_settings_dir()};
    const settings::Settings settings(tmp.path);
    EXPECT_FALSE(settings.getGeneralSettings().hasDefaultProfile());
    EXPECT_EQ(settings.getLoggingSettings().getLogDirectory(), "logs");
    EXPECT_DOUBLE_EQ(
        settings.getUISettings().getLogViewerSettings().getReloadIntervalSec(),
        1.0
    );
}

TEST(Settings, IsDirtyAfterConstruction)
{
    // GeneralSettings sets _version in its constructor but doesn't commit,
    // so the top-level isDirty() returns true.
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    EXPECT_TRUE(settings.isDirty());
}

TEST(Settings, KeyIsSettings)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    EXPECT_EQ(settings.getKey(), "settings");
}

TEST(Settings, SaveCreatesSettingsFile)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);

    const auto file = tmp.path / "settings.json";
    ASSERT_FALSE(std::filesystem::exists(file));

    settings.save();

    EXPECT_TRUE(std::filesystem::exists(file));
}

TEST(Settings, NotDirtyAfterSave)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    settings.save();
    EXPECT_FALSE(settings.isDirty());
}

TEST(Settings, SaveFiresOnSavedCallback)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);

    bool called = false;
    auto conn =
        settings.subscribeToSaved([&called]() { called = true; }, nullptr);

    settings.save();
    EXPECT_TRUE(called);
}

TEST(Settings, SubscribeToSavedCallbackNotFiredBeforeSave)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);

    bool called = false;
    auto conn =
        settings.subscribeToSaved([&called]() { called = true; }, nullptr);

    EXPECT_FALSE(called);
}

TEST(Settings, IsDirtyTrueAfterSettingDefaultProfile)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    settings.save();   // clear initial dirty state
    ASSERT_FALSE(settings.isDirty());

    settings.getGeneralSettings().setDefaultProfile("Dirty");
    EXPECT_TRUE(settings.isDirty());
}

TEST(Settings, IsDirtyFalseAfterSaveWhenDefaultProfileSet)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings(tmp.path);
    settings.getGeneralSettings().setDefaultProfile("Persisted");
    settings.save();
    EXPECT_FALSE(settings.isDirty());
}

TEST(Settings, SaveAndReloadDefaultProfilePreserved)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings1(tmp.path);
    settings1.getGeneralSettings().setDefaultProfile("Persisted");
    settings1.save();

    settings::Settings settings2(tmp.path);
    ASSERT_TRUE(settings2.getGeneralSettings().hasDefaultProfile());
    EXPECT_EQ(
        settings2.getGeneralSettings().getDefaultProfile().value(),
        "Persisted"
    );
}

TEST(Settings, SaveAndReloadNotDirtyAfterReload)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings1(tmp.path);
    settings1.getGeneralSettings().setDefaultProfile("Reload");
    settings1.save();

    settings::Settings settings2(tmp.path);
    EXPECT_FALSE(settings2.isDirty());
}

TEST(Settings, SaveAndReloadLogLevelPreserved)
{
    const TempDir      tmp{unique_temp_settings_dir()};
    settings::Settings settings1(tmp.path);
    const auto         result =
        settings1.getLoggingSettings().getDefaultLogLevelParam().set(
            LogLevel::Debug
        );
    ASSERT_TRUE(result.has_value());
    settings1.save();

    settings::Settings settings2(tmp.path);
    EXPECT_EQ(
        settings2.getLoggingSettings().getDefaultLogLevelParam().get(),
        LogLevel::Debug
    );
}
