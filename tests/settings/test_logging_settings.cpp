// tests/settings/test_logging_settings.cpp
//
// GoogleTest-based tests for settings::LoggingSettings.
//
// Coverage:
//  - Default values for all parameters
//  - isDirty is false after construction (all values via setDefault, not set)
//  - isDirty becomes true after modifying defaultLogLevel
//  - defaultLogLevel is not reboot-required (only param accessible via ref)
//  - getKey returns the schema key
//  - toJson contains expected parameter keys
//  - toJson / fromJson round-trip
//  - LoggingSettingsException message prefix

#include <gtest/gtest.h>

#include <cstddef>
#include <string>

#include "config/logging_base.hpp"
#include "settings/logging_settings.hpp"

TEST(LoggingSettings, DefaultLogDirectoryIsLogs)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getLogDirectory(), "logs");
}

TEST(LoggingSettings, DefaultLogFilePrefixIsMolarTracker)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getLogFilePrefix(), "molar_tracker_");
}

TEST(LoggingSettings, DefaultLogFileSuffixIsLog)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getLogFileSuffix(), ".log");
}

TEST(LoggingSettings, DefaultMaxLogFilesIs10)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getMaxLogFiles(), std::size_t{10});
}

TEST(LoggingSettings, DefaultMaxLogFileSizeMBIs50)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getMaxLogFileSizeMB(), std::size_t{50});
}

TEST(LoggingSettings, DefaultLogLevelIsInfo)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getDefaultLogLevelParam().get(), LogLevel::Info);
}

TEST(LoggingSettings, NotDirtyAfterConstruction)
{
    // All params are configured via setDefault(), not set(), so value and
    // baseline both remain nullopt -> not dirty.
    settings::LoggingSettings loggingSettings;
    EXPECT_FALSE(loggingSettings.isDirty());
}

TEST(LoggingSettings, DefaultLogLevelIsNotRebootRequired)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_FALSE(loggingSettings.getDefaultLogLevelParam().isRebootRequired());
}

TEST(LoggingSettings, KeyIsLoggingSettings)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getKey(), "loggingSettings");
}

TEST(LoggingSettings, DirtyAfterSettingLogLevel)
{
    settings::LoggingSettings loggingSettings;
    ASSERT_FALSE(loggingSettings.isDirty());

    const auto result =
        loggingSettings.getDefaultLogLevelParam().set(LogLevel::Debug);
    EXPECT_TRUE(result.has_value());
    EXPECT_TRUE(loggingSettings.isDirty());
}

TEST(LoggingSettings, NotDirtyAfterSettingThenCommitting)
{
    settings::LoggingSettings loggingSettings;
    const auto                result =
        loggingSettings.getDefaultLogLevelParam().set(LogLevel::Warning);
    EXPECT_TRUE(result.has_value());
    loggingSettings.commit();
    EXPECT_FALSE(loggingSettings.isDirty());
}

TEST(LoggingSettings, DefaultMaxLogAgeDaysIs30)
{
    settings::LoggingSettings loggingSettings;
    EXPECT_EQ(loggingSettings.getMaxLogAgeDays(), std::size_t{30});
}

TEST(LoggingSettings, ToJsonContainsAllParameterKeys)
{
    settings::LoggingSettings loggingSettings;
    const auto                json = loggingSettings.toJson();

    EXPECT_TRUE(json.contains("logDirectory"));
    EXPECT_TRUE(json.contains("logFilePrefix"));
    EXPECT_TRUE(json.contains("logFileSuffix"));
    EXPECT_TRUE(json.contains("maxLogFiles"));
    EXPECT_TRUE(json.contains("maxLogFileSizeMB"));
    EXPECT_TRUE(json.contains("maxLogAgeDays"));
    EXPECT_TRUE(json.contains("defaultLogLevel"));
}

TEST(LoggingSettings, FromJsonRoundTripLogLevelPreserved)
{
    settings::LoggingSettings ls1;
    const auto result1 = ls1.getDefaultLogLevelParam().set(LogLevel::Trace);
    EXPECT_TRUE(result1.has_value());
    const auto json = ls1.toJson();

    settings::LoggingSettings ls2;
    settings::LoggingSettings::fromJson(json, ls2);

    EXPECT_EQ(ls2.getDefaultLogLevelParam().get(), LogLevel::Trace);
}

TEST(LoggingSettings, FromJsonRoundTripDefaultsPreserved)
{
    settings::LoggingSettings ls1;
    const auto                json = ls1.toJson();

    settings::LoggingSettings ls2;
    settings::LoggingSettings::fromJson(json, ls2);

    EXPECT_EQ(ls2.getLogDirectory(), "logs");
    EXPECT_EQ(ls2.getLogFilePrefix(), "molar_tracker_");
    EXPECT_EQ(ls2.getLogFileSuffix(), ".log");
    EXPECT_EQ(ls2.getMaxLogFiles(), std::size_t{10});
    EXPECT_EQ(ls2.getMaxLogFileSizeMB(), std::size_t{50});
    EXPECT_EQ(ls2.getMaxLogAgeDays(), std::size_t{30});
}

TEST(LoggingSettings, FromJsonRoundTripNotDirtyAfterLoad)
{
    settings::LoggingSettings ls1;
    const auto result1 = ls1.getDefaultLogLevelParam().set(LogLevel::Error);
    EXPECT_TRUE(result1.has_value());
    const auto json = ls1.toJson();

    settings::LoggingSettings ls2;
    settings::LoggingSettings::fromJson(json, ls2);

    EXPECT_FALSE(ls2.isDirty());
}

TEST(LoggingSettingsException, MessageContainsPrefix)
{
    const settings::LoggingSettingsException exception("test error");
    const std::string                        msg = exception.what();
    EXPECT_NE(msg.find("LoggingSettingsException"), std::string::npos);
    EXPECT_NE(msg.find("test error"), std::string::npos);
}
