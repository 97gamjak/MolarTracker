#include <gtest/gtest.h>

#include <limits>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

#include "settings/params/param_core.hpp"
#include "settings/params/param_error.hpp"

// ============================================================================
// Construction
// ============================================================================

TEST(ParamCore, TwoArgConstructorStoresKeyAndTitle)
{
    settings::ParamCore<int> param("myKey", "My Title");
    EXPECT_EQ(param.getKey(), "myKey");
    EXPECT_EQ(param.getTitle(), "My Title");
    EXPECT_EQ(param.getDescription(), "");
}

TEST(ParamCore, ThreeArgConstructorStoresAllFields)
{
    settings::ParamCore<int> param("k", "T", "A description");
    EXPECT_EQ(param.getKey(), "k");
    EXPECT_EQ(param.getTitle(), "T");
    EXPECT_EQ(param.getDescription(), "A description");
}

// ============================================================================
// get / set / unset
// ============================================================================

TEST(ParamCore, GetThrowsWhenNoValueAndNoDefault)
{
    settings::ParamCore<int> param("k", "T", "D");
    EXPECT_THROW((void) param.get(), settings::ParamException);
}

TEST(ParamCore, GetReturnsValueAfterSet)
{
    const auto               paramValue = 42;
    settings::ParamCore<int> param("k", "T", "D");
    param.set(paramValue);
    EXPECT_EQ(param.get(), paramValue);
}

TEST(ParamCore, GetReturnsDefaultWhenValueNotSet)
{
    const auto               defaultValue = 99;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(defaultValue);
    EXPECT_EQ(param.get(), defaultValue);
}

TEST(ParamCore, SetOverwritesPreviousValue)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.set(1);
    param.set(2);
    EXPECT_EQ(param.get(), 2);
}

TEST(ParamCore, SetToDefaultValueSkipsSilently)
{
    // If the current effective value equals the new value, set is a no-op.
    // Setting to the default when _value is null leaves _value as null.
    const auto               value = 42;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(value);
    param.set(value);   // same as default -> early return, _value stays null
    EXPECT_FALSE(param.isDirty());
    EXPECT_EQ(param.get(), value);   // still returns default
}

TEST(ParamCore, UnsetClearsValueFallsBackToDefault)
{
    const auto               paramValue = 42;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);   // required to avoid throw in _notifySubscribers
    param.set(paramValue);
    EXPECT_EQ(param.get(), paramValue);

    param.unset();
    EXPECT_EQ(param.get(), 0);   // falls back to default
}

TEST(ParamCore, ResetToDefaultClearsValueWhenDefaultConfigured)
{
    const auto               defaultValue = 7;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(defaultValue);
    constexpr auto paramValue = 42;
    param.set(paramValue);
    EXPECT_EQ(param.get(), paramValue);

    param.resetToDefault();
    EXPECT_EQ(param.get(), defaultValue);
}

TEST(ParamCore, ResetToDefaultIsNoOpWhenNoDefaultConfigured)
{
    const auto               paramValue = 42;
    settings::ParamCore<int> param("k", "T", "D");
    param.set(paramValue);

    param.resetToDefault();
    EXPECT_EQ(param.get(), paramValue);
}

// ============================================================================
// getOptional
// ============================================================================

TEST(ParamCore, GetOptionalReturnsNulloptWhenNothingSet)
{
    settings::ParamCore<int> param("k", "T", "D");
    EXPECT_FALSE(param.getOptional().has_value());
}

TEST(ParamCore, GetOptionalReturnsValueWhenSet)
{
    const auto               paramValue = 7;
    settings::ParamCore<int> param("k", "T", "D");
    param.set(paramValue);
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), paramValue);
}

TEST(ParamCore, GetOptionalFallsBackToDefaultWhenValueNotSet)
{
    const auto               defaultValue = 55;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(defaultValue);
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), defaultValue);
}

TEST(ParamCore, GetOptionalReturnsNulloptAfterUnsetWithNoDefault)
{
    // After unset with no default the optional is empty.
    // Note: calling unset() without a default ALSO throws inside
    // _notifySubscribers, so we must provide a default.
    const auto               paramValue   = 5;
    const auto               defaultValue = 0;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(defaultValue);
    param.set(paramValue);
    param.unset();
    // _value is now null; getOptional falls back to default
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), defaultValue);
}

// ============================================================================
// isDirty / commit
// ============================================================================

TEST(ParamCore, IsDirtyFalseOnFreshParam)
{
    settings::ParamCore<int> param("k", "T", "D");
    EXPECT_FALSE(param.isDirty());
}

TEST(ParamCore, IsDirtyTrueAfterSet)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.set(1);
    EXPECT_TRUE(param.isDirty());
}

TEST(ParamCore, IsDirtyFalseAfterSetAndCommit)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.set(1);
    param.commit();
    EXPECT_FALSE(param.isDirty());
}

TEST(ParamCore, IsDirtyTrueAfterCommitThenNewValue)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.set(1);
    param.commit();
    param.set(2);
    EXPECT_TRUE(param.isDirty());
}

TEST(ParamCore, IsDirtyTrueAfterUnsetFollowingCommit)
{
    const auto               paramValue = 10;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);   // prevent throw in _notifySubscribers
    param.set(paramValue);
    param.commit();   // baseline = 10
    param.unset();    // value = null, baseline = 10
    EXPECT_TRUE(param.isDirty());
}

TEST(ParamCore, IsDirtyFalseAfterUnsetWithoutPriorCommit)
{
    // set then unset without commit: baseline stays null, value returns to null
    const auto               paramValue = 10;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);
    param.set(paramValue);
    EXPECT_TRUE(param.isDirty());
    param.unset();   // both baseline and value are null
    EXPECT_FALSE(param.isDirty());
}

// ============================================================================
// default value
// ============================================================================

TEST(ParamCore, SetDefaultStoresAndReturnsValue)
{
    const auto               defaultValue = 77;
    settings::ParamCore<int> param("k", "T", "D");
    EXPECT_FALSE(param.getDefault().has_value());

    param.setDefault(defaultValue);
    ASSERT_TRUE(param.getDefault().has_value());
    EXPECT_EQ(param.getDefault().value(), defaultValue);
}

TEST(ParamCore, SetDefaultToNulloptClearsDefault)
{
    const auto               paramValue = 10;
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(paramValue);
    param.setDefault(std::nullopt);
    EXPECT_FALSE(param.getDefault().has_value());
}

// ============================================================================
// description
// ============================================================================

TEST(ParamCore, SetDescriptionUpdatesValue)
{
    settings::ParamCore<int> param("k", "T", "Initial");
    param.setDescription("Updated description");
    EXPECT_EQ(param.getDescription(), "Updated description");
}

// ============================================================================
// reboot required
// ============================================================================

TEST(ParamCore, RebootRequiredFalseByDefault)
{
    settings::ParamCore<int> param("k", "T", "D");
    EXPECT_FALSE(param.isRebootRequired());
}

TEST(ParamCore, SetRebootRequiredToTrue)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setRebootRequired(true);
    EXPECT_TRUE(param.isRebootRequired());
}

TEST(ParamCore, SetRebootRequiredBackToFalse)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setRebootRequired(true);
    param.setRebootRequired(false);
    EXPECT_FALSE(param.isRebootRequired());
}

// ============================================================================
// subscribe
// ============================================================================

TEST(ParamCore, SubscribeCallbackFiresOnValueChange)
{
    settings::ParamCore<int> param("k", "T", "D");
    int                      received   = -1;
    const auto               paramValue = 42;

    auto conn =
        param.subscribe([&](const int& val) { received = val; }, nullptr);
    param.set(paramValue);
    EXPECT_EQ(received, paramValue);
}

TEST(ParamCore, SubscribeCallbackNotFiredForSameValue)
{
    settings::ParamCore<int> param("k", "T", "D");
    int                      callCount = 0;

    const auto paramValue = 5;
    param.set(paramValue);
    auto conn =
        param.subscribe([&](const int& /*val*/) { callCount++; }, nullptr);
    param.set(paramValue);   // same value, no notification
    EXPECT_EQ(callCount, 0);
}

TEST(ParamCore, SubscribeToOptionalFiresOnChange)
{
    settings::ParamCore<int> param("k", "T", "D");
    std::optional<int>       last = std::nullopt;

    auto conn = param.subscribeToOptional(
        [&](const std::optional<int>& value) { last = value; },
        nullptr
    );
    const auto paramValue = 10;
    param.set(paramValue);
    ASSERT_TRUE(last.has_value());
    EXPECT_EQ(last.value(), paramValue);
}

TEST(ParamCore, SubscribeToDirtyFiresWithCorrectState)
{
    settings::ParamCore<int> param("k", "T", "D");
    bool                     lastDirty = false;

    auto conn = param.subscribeToDirty(
        [&](const bool& value) { lastDirty = value; },
        nullptr
    );
    param.set(1);
    EXPECT_TRUE(lastDirty);

    param.commit();
    param.set(1);             // same value after commit -> no notification
    EXPECT_TRUE(lastDirty);   // unchanged from last notification
}

TEST(ParamCore, SubscribeThrowsWhenRebootRequired)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setRebootRequired(true);
    EXPECT_THROW(
        (void) param.subscribe([](const int&) {}, nullptr),
        settings::ParamException
    );
}

TEST(ParamCore, SubscribeToOptionalThrowsWhenRebootRequired)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setRebootRequired(true);
    EXPECT_THROW(
        (void
        ) param.subscribeToOptional([](const std::optional<int>&) {}, nullptr),
        settings::ParamException
    );
}

// ============================================================================
// JSON serialization
// ============================================================================

TEST(ParamCore, ToJsonContainsExpectedKeys)
{
    settings::ParamCore<int> param("theKey", "The Title", "The Desc");
    const auto               paramValue = 7;
    param.set(paramValue);
    param.setDefault(0);

    auto json = param.toJson();
    EXPECT_TRUE(json.contains("key"));
    EXPECT_TRUE(json.contains("title"));
    EXPECT_TRUE(json.contains("description"));
    EXPECT_TRUE(json.contains("value"));
    EXPECT_TRUE(json.contains("default"));
}

TEST(ParamCore, ToJsonRoundTripInt)
{
    settings::ParamCore<int> original("k", "T", "D");
    const auto               paramValue = 42;
    original.set(paramValue);
    original.setDefault(0);

    auto json = original.toJson();

    settings::ParamCore<int> restored("", "", "");
    settings::ParamCore<int>::fromJson(json, restored);

    EXPECT_EQ(restored.getKey(), "k");
    EXPECT_EQ(restored.getTitle(), "T");
    EXPECT_EQ(restored.getDescription(), "D");
    EXPECT_EQ(restored.get(), paramValue);
    ASSERT_TRUE(restored.getDefault().has_value());
    EXPECT_EQ(restored.getDefault().value(), 0);
}

TEST(ParamCore, ToJsonRoundTripString)
{
    settings::ParamCore<std::string> original("sk", "ST", "SD");
    original.set(std::string("hello"));
    original.setDefault(std::string("world"));

    auto json = original.toJson();

    settings::ParamCore<std::string> restored("", "", "");
    settings::ParamCore<std::string>::fromJson(json, restored);

    EXPECT_EQ(restored.get(), "hello");
    ASSERT_TRUE(restored.getDefault().has_value());
    EXPECT_EQ(restored.getDefault().value(), "world");
}

TEST(ParamCore, FromJsonSetsBaselineToValue)
{
    settings::ParamCore<int> param("k", "T", "D");
    const auto               paramValue = 99;
    param.set(paramValue);
    auto json = param.toJson();

    settings::ParamCore<int> restored("", "", "");
    settings::ParamCore<int>::fromJson(json, restored);

    // After fromJson, baseline = value, so not dirty
    EXPECT_FALSE(restored.isDirty());
}

TEST(ParamCore, ToJsonNullValueSerialized)
{
    settings::ParamCore<int> param("k", "T", "D");
    // no value set, no default

    auto json = param.toJson();
    EXPECT_TRUE(json["value"].is_null());
    EXPECT_TRUE(json["default"].is_null());
}

// ============================================================================
// Floating-point equality
// ============================================================================

TEST(ParamCore, FloatSetWithEpsilonDiffDoesNotDirty)
{
    settings::ParamCore<double> param("fp", "FP", "D");
    const double                val = 1.0;
    param.set(val);
    param.commit();

    const double eps = std::numeric_limits<double>::epsilon();
    param.set(val + eps);   // difference == epsilon => equal => no-op
    EXPECT_FALSE(param.isDirty());
}

TEST(ParamCore, FloatSetBeyondEpsilonDirtiesParam)
{
    settings::ParamCore<double> param("fp", "FP", "D");
    const double                val = 1.0;
    param.set(val);
    param.commit();

    const auto   scaling = 2.0;
    const double eps     = std::numeric_limits<double>::epsilon();
    param.set(val + (scaling * eps));   // difference > epsilon => different
    EXPECT_TRUE(param.isDirty());
}
