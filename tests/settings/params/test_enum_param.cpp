#include <gtest/gtest.h>

#include <cstdint>
#include <optional>
#include <string>

#include "config/logging_base.hpp"
#include "settings/params/enum_param.hpp"
#include "settings/params/param_error.hpp"

// ============================================================================
// Construction / metadata
// ============================================================================

TEST(EnumParam, StoresKeyTitleDescription)
{
    settings::EnumParam<LogLevel> param("lvl", "Log Level", "The log level");
    EXPECT_EQ(param.getKey(), "lvl");
    EXPECT_EQ(param.getTitle(), "Log Level");
    EXPECT_EQ(param.getDescription(), "The log level");
}

// ============================================================================
// set / get
// ============================================================================

TEST(EnumParam, SetValidEnumValueSucceeds)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    auto                          result = param.set(LogLevel::Info);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), LogLevel::Info);
}

TEST(EnumParam, SetAllValidValuesSucceed)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");

    for (const auto& val : LogLevelMeta::values_view())
    {
        auto result = param.set(val);
        EXPECT_TRUE(result.has_value()) << "Failed for enum value index";
        EXPECT_EQ(param.get(), val);
    }
}

TEST(EnumParam, SetInvalidEnumValueReturnsError)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    // Cast an out-of-range integer to LogLevel to produce an invalid value
    const auto invalid =
        static_cast<LogLevel>(static_cast<std::int8_t>(127));
    auto result = param.set(invalid);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().getMessage().empty());
}

TEST(EnumParam, GetThrowsWhenNoValueAndNoDefault)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    EXPECT_THROW((void) param.get(), settings::ParamException);
}

TEST(EnumParam, GetReturnsDefaultWhenValueNotSet)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    param.setDefault(std::optional<LogLevel>(LogLevel::Warning));
    EXPECT_EQ(param.get(), LogLevel::Warning);
}

// ============================================================================
// getOptional
// ============================================================================

TEST(EnumParam, GetOptionalNulloptOnFreshParam)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    EXPECT_FALSE(param.getOptional().has_value());
}

TEST(EnumParam, GetOptionalReturnsValueAfterSet)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    auto                          r = param.set(LogLevel::Debug);
    ASSERT_TRUE(r.has_value());
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), LogLevel::Debug);
}

// ============================================================================
// isDirty / commit
// ============================================================================

TEST(EnumParam, IsDirtyFalseOnFreshParam)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    EXPECT_FALSE(param.isDirty());
}

TEST(EnumParam, IsDirtyTrueAfterSet)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    auto                          r = param.set(LogLevel::Error);
    ASSERT_TRUE(r.has_value());
    EXPECT_TRUE(param.isDirty());
}

TEST(EnumParam, CommitClearsDirty)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    auto                          r = param.set(LogLevel::Trace);
    ASSERT_TRUE(r.has_value());
    param.commit();
    EXPECT_FALSE(param.isDirty());
}

TEST(EnumParam, IsDirtyTrueAfterCommitAndNewValue)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    auto                          r1 = param.set(LogLevel::Off);
    ASSERT_TRUE(r1.has_value());
    param.commit();

    auto r2 = param.set(LogLevel::Error);
    ASSERT_TRUE(r2.has_value());
    EXPECT_TRUE(param.isDirty());
}

// ============================================================================
// reboot required
// ============================================================================

TEST(EnumParam, RebootRequiredFalseByDefault)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    EXPECT_FALSE(param.isRebootRequired());
}

TEST(EnumParam, SetRebootRequiredPreventsSubscribe)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    param.setRebootRequired(true);
    EXPECT_THROW(
        (void) param.subscribe(
            [](const LogLevel&) {},
            nullptr
        ),
        settings::ParamException
    );
}

// ============================================================================
// JSON serialisation
// ============================================================================

TEST(EnumParam, ToJsonRoundTrip)
{
    settings::EnumParam<LogLevel> original("lvl", "LT", "LD");
    auto                          r = original.set(LogLevel::Warning);
    ASSERT_TRUE(r.has_value());

    auto json = original.toJson();

    settings::EnumParam<LogLevel> restored("", "", "");
    settings::EnumParam<LogLevel>::fromJson(json, restored);

    EXPECT_EQ(restored.getKey(), "lvl");
    EXPECT_EQ(restored.get(), LogLevel::Warning);
    EXPECT_FALSE(restored.isDirty());   // baseline = value after fromJson
}

TEST(EnumParam, ToJsonNullValueSerialised)
{
    settings::EnumParam<LogLevel> param("lvl", "L", "D");
    auto                          json = param.toJson();
    EXPECT_TRUE(json.contains("value"));
    EXPECT_TRUE(json["value"].is_null());
}
