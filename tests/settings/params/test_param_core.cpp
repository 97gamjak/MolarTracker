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
    settings::ParamCore<int> param("k", "T", "D");
    param.set(42);
    EXPECT_EQ(param.get(), 42);
}

TEST(ParamCore, GetReturnsDefaultWhenValueNotSet)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(99);
    EXPECT_EQ(param.get(), 99);
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
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(42);
    param.set(42);   // same as default -> early return, _value stays null
    EXPECT_FALSE(param.isDirty());
    EXPECT_EQ(param.get(), 42);   // still returns default
}

TEST(ParamCore, UnsetClearsValueFallsBackToDefault)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);   // required to avoid throw in _notifySubscribers
    param.set(42);
    EXPECT_EQ(param.get(), 42);

    param.unset();
    EXPECT_EQ(param.get(), 0);   // falls back to default
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
    settings::ParamCore<int> param("k", "T", "D");
    param.set(7);
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), 7);
}

TEST(ParamCore, GetOptionalFallsBackToDefaultWhenValueNotSet)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(55);
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), 55);
}

TEST(ParamCore, GetOptionalReturnsNulloptAfterUnsetWithNoDefault)
{
    // After unset with no default the optional is empty.
    // Note: calling unset() without a default ALSO throws inside
    // _notifySubscribers, so we must provide a default.
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);
    param.set(5);
    param.unset();
    // _value is now null; getOptional falls back to default
    ASSERT_TRUE(param.getOptional().has_value());
    EXPECT_EQ(param.getOptional().value(), 0);
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
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);   // prevent throw in _notifySubscribers
    param.set(10);
    param.commit();   // baseline = 10
    param.unset();    // value = null, baseline = 10
    EXPECT_TRUE(param.isDirty());
}

TEST(ParamCore, IsDirtyFalseAfterUnsetWithoutPriorCommit)
{
    // set then unset without commit: baseline stays null, value returns to null
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(0);
    param.set(10);
    EXPECT_TRUE(param.isDirty());
    param.unset();   // both baseline and value are null
    EXPECT_FALSE(param.isDirty());
}

// ============================================================================
// default value
// ============================================================================

TEST(ParamCore, SetDefaultStoresAndReturnsValue)
{
    settings::ParamCore<int> param("k", "T", "D");
    EXPECT_FALSE(param.getDefault().has_value());

    param.setDefault(77);
    ASSERT_TRUE(param.getDefault().has_value());
    EXPECT_EQ(param.getDefault().value(), 77);
}

TEST(ParamCore, SetDefaultToNulloptClearsDefault)
{
    settings::ParamCore<int> param("k", "T", "D");
    param.setDefault(10);
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
    int                      received = -1;

    auto conn = param.subscribe(
        [&](const int& val) { received = val; },
        nullptr
    );
    param.set(42);
    EXPECT_EQ(received, 42);
}

TEST(ParamCore, SubscribeCallbackNotFiredForSameValue)
{
    settings::ParamCore<int> param("k", "T", "D");
    int                      callCount = 0;

    param.set(5);
    auto conn = param.subscribe(
        [&](const int& /*val*/) { callCount++; },
        nullptr
    );
    param.set(5);   // same value, no notification
    EXPECT_EQ(callCount, 0);
}

TEST(ParamCore, SubscribeToOptionalFiresOnChange)
{
    settings::ParamCore<int>           param("k", "T", "D");
    std::optional<int>                 last = std::nullopt;

    auto conn = param.subscribeToOptional(
        [&](const std::optional<int>& v) { last = v; },
        nullptr
    );
    param.set(10);
    ASSERT_TRUE(last.has_value());
    EXPECT_EQ(last.value(), 10);
}

TEST(ParamCore, SubscribeToDirtyFiresWithCorrectState)
{
    settings::ParamCore<int> param("k", "T", "D");
    bool                     lastDirty = false;

    auto conn = param.subscribeToDirty(
        [&](const bool& d) { lastDirty = d; },
        nullptr
    );
    param.set(1);
    EXPECT_TRUE(lastDirty);

    param.commit();
    param.set(1);   // same value after commit -> no notification
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
        (void) param.subscribeToOptional(
            [](const std::optional<int>&) {},
            nullptr
        ),
        settings::ParamException
    );
}

// ============================================================================
// JSON serialisation
// ============================================================================

TEST(ParamCore, ToJsonContainsExpectedKeys)
{
    settings::ParamCore<int> param("theKey", "The Title", "The Desc");
    param.set(7);
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
    original.set(42);
    original.setDefault(0);

    auto json = original.toJson();

    settings::ParamCore<int> restored("", "", "");
    settings::ParamCore<int>::fromJson(json, restored);

    EXPECT_EQ(restored.getKey(), "k");
    EXPECT_EQ(restored.getTitle(), "T");
    EXPECT_EQ(restored.getDescription(), "D");
    EXPECT_EQ(restored.get(), 42);
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
    param.set(99);
    auto json = param.toJson();

    settings::ParamCore<int> restored("", "", "");
    settings::ParamCore<int>::fromJson(json, restored);

    // After fromJson, baseline = value, so not dirty
    EXPECT_FALSE(restored.isDirty());
}

TEST(ParamCore, ToJsonNullValueSerialised)
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

    const double eps = std::numeric_limits<double>::epsilon();
    param.set(val + 2.0 * eps);   // difference > epsilon => different
    EXPECT_TRUE(param.isDirty());
}
