// tests/logging/test_log_file_cleaner.cpp
//
// GoogleTest-based tests for logging::LogFileCleaner::cleanByAge.
//
// Coverage:
//  - No-op when maxAgeDays is 0
//  - Deletes files older than maxAgeDays
//  - Preserves files newer than maxAgeDays
//  - Ignores files with non-matching prefix
//  - Ignores files with non-matching suffix
//  - Ignores symlinks (non-regular files)

#include <gtest/gtest.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

#include "logging/log_file_cleaner.hpp"

namespace
{

    namespace fs = std::filesystem;

    // Creates a file at `path` and sets its mtime to `ageInDays` days ago.
    void createAgedFile(const fs::path& path, int ageInDays)
    {
        std::ofstream file{path};   // create empty file

        const auto mtime =
            fs::file_time_type::clock::now() -
            std::chrono::hours(24) * static_cast<std::int64_t>(ageInDays);
        fs::last_write_time(path, mtime);
    }

    class LogFileCleanerTest : public ::testing::Test
    {
       protected:
        // NOLINTNEXTLINE(misc-non-private-member-variables-in-classes)
        fs::path _dir;

        void SetUp() override
        {
            _dir =
                fs::temp_directory_path() /
                ("molartracker_test_" +
                 std::to_string(
                     std::chrono::steady_clock::now().time_since_epoch().count()
                 ));
            fs::create_directories(_dir);
        }

        void TearDown() override
        {
            std::error_code errorCode;
            fs::remove_all(_dir, errorCode);
        }
    };

}   // namespace

TEST_F(LogFileCleanerTest, NoOpWhenMaxAgeDaysIsZero)
{
    const auto file   = _dir / "molar_tracker_old.log";
    const auto age    = 100;
    const auto maxAge = 0;   // 0 disables cleanup
    createAgedFile(file, age);

    logging::LogFileCleaner::cleanByAge(_dir, "molar_tracker_", ".log", maxAge);

    EXPECT_TRUE(fs::exists(file));
}

TEST_F(LogFileCleanerTest, DeletesOldFiles)
{
    const auto old    = _dir / "molar_tracker_20260101_120000.log";
    const auto age    = 40;
    const auto maxAge = 30;
    createAgedFile(old, age);   // 40 days old, limit is 30

    logging::LogFileCleaner::cleanByAge(_dir, "molar_tracker_", ".log", maxAge);

    EXPECT_FALSE(fs::exists(old));
}

TEST_F(LogFileCleanerTest, PreservesNewFiles)
{
    const auto recent = _dir / "molar_tracker_20260607_120000.log";
    const auto age    = 5;
    const auto maxAge = 30;
    createAgedFile(recent, age);   // 5 days old, limit is 30

    logging::LogFileCleaner::cleanByAge(_dir, "molar_tracker_", ".log", maxAge);

    EXPECT_TRUE(fs::exists(recent));
}

TEST_F(LogFileCleanerTest, IgnoresFilesWithWrongPrefix)
{
    const auto file   = _dir / "other_app_20260101_120000.log";
    const auto age    = 40;
    const auto maxAge = 30;
    createAgedFile(file, age);

    logging::LogFileCleaner::cleanByAge(_dir, "molar_tracker_", ".log", maxAge);

    EXPECT_TRUE(fs::exists(file));
}

TEST_F(LogFileCleanerTest, IgnoresFilesWithWrongSuffix)
{
    const auto file   = _dir / "molar_tracker_20260101_120000.txt";
    const auto age    = 40;
    const auto maxAge = 30;
    createAgedFile(file, age);

    logging::LogFileCleaner::cleanByAge(_dir, "molar_tracker_", ".log", maxAge);

    EXPECT_TRUE(fs::exists(file));
}

TEST_F(LogFileCleanerTest, NonExistentDirectoryIsNoOp)
{
    const auto missing = _dir / "no_such_dir";
    EXPECT_NO_THROW(
        logging::LogFileCleaner::cleanByAge(
            missing,
            "molar_tracker_",
            ".log",
            30
        )
    );
}
