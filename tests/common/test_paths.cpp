// tests/utils/test_paths.cpp
//
// GoogleTest-based tests for common::paths functions.
//
// Coverage:
//  - user_dir: valid app name returns a non-empty path
//  - user_dir: invalid app names return PathError::InvalidAppName
//  - ensure_dir: creates a directory and returns the path
//  - ensure_dir: returns PathError::Empty for an empty path
//  - config_dir: creates the config directory and returns the path
//  - data_dir: creates the data directory and returns the path

#include <gtest/gtest.h>

#include <filesystem>
#include <random>
#include <string>

#include "common/paths.hpp"

namespace
{
    std::filesystem::path unique_temp_subdir(std::string_view stem)
    {
        const auto tmp = std::filesystem::temp_directory_path();

        std::random_device                           randomDevice;
        std::mt19937_64                              gen(randomDevice());
        std::uniform_int_distribution<std::uint64_t> dis;

        const auto random = dis(gen);

        return tmp / (std::string(stem) + "_" + std::to_string(random));
    }
}   // namespace

TEST(UtilsPaths, UserDirConfigReturnsValidPath)
{
    const auto result = common::userDir(common::DirKind::Config, "TestApp");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value().empty());
    EXPECT_TRUE(result.value().string().find("TestApp") != std::string::npos);
}

TEST(UtilsPaths, UserDirDataReturnsValidPath)
{
    const auto result = common::userDir(common::DirKind::Data, "TestApp");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value().empty());
    EXPECT_TRUE(result.value().string().find("TestApp") != std::string::npos);
}

TEST(UtilsPaths, UserDirEmptyAppNameReturnsInvalidAppName)
{
    const auto result = common::userDir(common::DirKind::Config, "");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), common::PathError::InvalidAppName);
}

TEST(UtilsPaths, UserDirAppNameWithSlashReturnsInvalidAppName)
{
    const auto result = common::userDir(common::DirKind::Config, "bad/name");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), common::PathError::InvalidAppName);
}

TEST(UtilsPaths, UserDirAppNameWithDotDotReturnsInvalidAppName)
{
    const auto result = common::userDir(common::DirKind::Config, "..");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), common::PathError::InvalidAppName);
}

TEST(UtilsPaths, EnsureDirCreatesDirectoryAndReturnsPath)
{
    const auto dir = unique_temp_subdir("molartracker_ensure_dir_test");

    EXPECT_FALSE(std::filesystem::exists(dir));

    const auto result = common::ensureDir(dir);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), dir);
    EXPECT_TRUE(std::filesystem::is_directory(dir));

    std::error_code errorCode;
    std::filesystem::remove_all(dir, errorCode);
}

TEST(UtilsPaths, EnsureDirEmptyPathReturnsEmpty)
{
    const auto result = common::ensureDir(std::filesystem::path{});
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), common::PathError::Empty);
}

TEST(UtilsPaths, EnsureDirIdempotentOnExistingDirectory)
{
    const auto dir = unique_temp_subdir("molartracker_ensure_dir_idempotent");

    std::filesystem::create_directories(dir);
    ASSERT_TRUE(std::filesystem::is_directory(dir));

    const auto result = common::ensureDir(dir);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), dir);

    std::error_code errorCode;
    std::filesystem::remove_all(dir, errorCode);
}

TEST(UtilsPaths, ConfigDirCreatesDirectoryAndReturnsPath)
{
    const auto result = common::configDir("MolarTrackerTest");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value().empty());
    EXPECT_TRUE(std::filesystem::is_directory(result.value()));

    std::error_code errorCode;
    std::filesystem::remove_all(result.value(), errorCode);
}

TEST(UtilsPaths, DataDirCreatesDirectoryAndReturnsPath)
{
    const auto result = common::dataDir("MolarTrackerTest");
    ASSERT_TRUE(result.has_value());
    EXPECT_FALSE(result.value().empty());
    EXPECT_TRUE(std::filesystem::is_directory(result.value()));

    std::error_code errorCode;
    std::filesystem::remove_all(result.value(), errorCode);
}

TEST(UtilsPaths, ConfigDirInvalidAppNameReturnsInvalidAppName)
{
    const auto result = common::configDir("");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), common::PathError::InvalidAppName);
}

TEST(UtilsPaths, DataDirInvalidAppNameReturnsInvalidAppName)
{
    const auto result = common::dataDir("bad/name");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), common::PathError::InvalidAppName);
}
