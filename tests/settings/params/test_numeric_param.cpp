#include <gtest/gtest.h>

#include <cmath>
#include <cstddef>
#include <numbers>
#include <optional>
#include <string>

#include "settings/params/numeric_param.hpp"
#include "settings/params/param_error.hpp"

// ============================================================================
// Construction / metadata
// ============================================================================

TEST(NumericParam, StoresKeyTitleDescription)
{
    settings::NumericParam<int> param("nKey", "N Title", "N Desc");
    EXPECT_EQ(param.getKey(), "nKey");
    EXPECT_EQ(param.getTitle(), "N Title");
    EXPECT_EQ(param.getDescription(), "N Desc");
}

TEST(NumericParam, NoLimitsByDefault)
{
    settings::NumericParam<int> param("k", "T", "D");
    EXPECT_FALSE(param.getMinValue().has_value());
    EXPECT_FALSE(param.getMaxValue().has_value());
}

TEST(NumericParam, NoPrecisionByDefault)
{
    settings::NumericParam<int>    intParam("k", "T", "D");
    settings::NumericParam<double> dblParam("k", "T", "D");
    EXPECT_FALSE(intParam.getPrecision().has_value());
    EXPECT_FALSE(dblParam.getPrecision().has_value());
}

// ============================================================================
// set / get / isDirty / commit
// ============================================================================

TEST(NumericParam, SetValidValueSucceeds)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value  = 5;
    auto                        result = param.set(value);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), value);
}

TEST(NumericParam, IsDirtyFalseBeforeSet)
{
    settings::NumericParam<int> param("k", "T", "D");
    EXPECT_FALSE(param.isDirty());
}

TEST(NumericParam, IsDirtyTrueAfterSet)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value  = 10;
    auto                        result = param.set(value);
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(param.isDirty());
}

TEST(NumericParam, CommitClearsDirty)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value  = 10;
    auto                        result = param.set(value);
    ASSERT_TRUE(result.has_value());
    param.commit();
    EXPECT_FALSE(param.isDirty());
}

// ============================================================================
// Default value
// ============================================================================

TEST(NumericParam, SetDefaultAndGetDefault)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value = 100;
    param.setDefault(std::optional<int>(value));
    ASSERT_TRUE(param.getDefault().has_value());
    EXPECT_EQ(param.getDefault().value(), value);
}

// ============================================================================
// Range limits
// ============================================================================

TEST(NumericParam, SetLimitsStoresMinMax)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value = 100;
    param.setLimits(0, value);
    ASSERT_TRUE(param.getMinValue().has_value());
    ASSERT_TRUE(param.getMaxValue().has_value());
    EXPECT_EQ(param.getMinValue().value(), 0);
    EXPECT_EQ(param.getMaxValue().value(), value);
}

TEST(NumericParam, SetMinValueStoresMin)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value = 5;
    param.setMinValue(value);
    ASSERT_TRUE(param.getMinValue().has_value());
    EXPECT_EQ(param.getMinValue().value(), value);
}

TEST(NumericParam, SetMaxValueStoresMax)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  value = 200;
    param.setMaxValue(value);
    ASSERT_TRUE(param.getMaxValue().has_value());
    EXPECT_EQ(param.getMaxValue().value(), value);
}

TEST(NumericParam, SetBelowMinReturnsError)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  limit1 = 10;
    const auto                  limit2 = 100;
    const auto                  value  = 9;
    param.setLimits(limit1, limit2);
    auto result = param.set(value);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().getMessage().empty());
}

TEST(NumericParam, SetAboveMaxReturnsError)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  limit1 = 0;
    const auto                  limit2 = 50;
    const auto                  value  = 51;
    param.setLimits(limit1, limit2);
    auto result = param.set(value);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().getMessage().empty());
}

TEST(NumericParam, SetAtMinBoundarySucceeds)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  limit1 = 0;
    const auto                  limit2 = 100;
    param.setLimits(limit1, limit2);
    auto result = param.set(limit1);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), limit1);
}

TEST(NumericParam, SetAtMaxBoundarySucceeds)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  limit1 = 0;
    const auto                  limit2 = 100;
    param.setLimits(limit1, limit2);
    auto result = param.set(limit2);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), limit2);
}

TEST(NumericParam, SetWithOnlyMinAllowsAboveMin)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  minValue = 5;
    param.setMinValue(minValue);
    EXPECT_TRUE(param.set(1000).has_value());
}

TEST(NumericParam, SetWithOnlyMinRejectsBelow)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  minValue = 5;
    param.setMinValue(minValue);
    EXPECT_FALSE(param.set(4).has_value());
}

TEST(NumericParam, SetWithOnlyMaxAllowsBelowMax)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  maxValue = 100;
    param.setMaxValue(maxValue);
    EXPECT_TRUE(param.set(-9999).has_value());
}

TEST(NumericParam, SetWithOnlyMaxRejectsAbove)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  maxValue = 100;
    param.setMaxValue(maxValue);
    EXPECT_FALSE(param.set(101).has_value());
}

// ============================================================================
// Precision (floating-point)
// ============================================================================

TEST(NumericParam, SetPrecisionRoundsDouble)
{
    settings::NumericParam<double> param("k", "T", "D");
    param.setPrecision(2);   // 2 decimal places
    const auto piValue = std::numbers::pi;
    auto       result  = param.set(piValue);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(param.get(), 3.14, 1e-9);
}

TEST(NumericParam, SetPrecisionRoundsUp)
{
    settings::NumericParam<double> param("k", "T", "D");
    param.setPrecision(1);
    const auto value  = 1.95;
    auto       result = param.set(value);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(param.get(), 2.0, 1e-9);
}

TEST(NumericParam, GetPrecisionAfterSet)
{
    settings::NumericParam<double> param("k", "T", "D");
    param.setPrecision(3);
    ASSERT_TRUE(param.getPrecision().has_value());
    EXPECT_EQ(param.getPrecision().value(), static_cast<std::size_t>(3));
}

TEST(NumericParam, SetPrecisionOnIntIsNoOp)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setPrecision(2);
    EXPECT_FALSE(param.getPrecision().has_value());
}

TEST(NumericParam, PrecisionAppliedBeforeRangeCheck)
{
    // After rounding to 2 dp, 2.005 becomes 2.01 which is within [0, 10]
    settings::NumericParam<double> param("k", "T", "D");
    const auto                     limit1    = 0.0;
    const auto                     limit2    = 10.0;
    const auto                     precision = 2;
    const auto                     value     = 2.005;
    param.setPrecision(precision);
    param.setLimits(limit1, limit2);
    auto result = param.set(value);
    EXPECT_TRUE(result.has_value());
}

// ============================================================================
// subscribe (via ParamMixin — raw function pointer)
// ============================================================================

namespace
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    int  s_numericIntValue = 0;
    void onNumericIntChanged(const int& value) { s_numericIntValue = value; }
}   // namespace

TEST(NumericParam, SubscribeCallbackFiredOnValidSet)
{
    s_numericIntValue = -1;
    settings::NumericParam<int> param("k", "T", "D");
    auto conn = param.subscribe(onNumericIntChanged, nullptr);

    const auto value  = 55;
    auto       result = param.set(value);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(s_numericIntValue, value);
}

TEST(NumericParam, SubscribeCallbackNotFiredOnOutOfRangeSet)
{
    s_numericIntValue = -1;
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  limits = std::pair(0, 10);
    param.setLimits(limits.first, limits.second);
    auto conn = param.subscribe(onNumericIntChanged, nullptr);

    const auto value  = 99;
    auto       result = param.set(value);   // out of range
    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(s_numericIntValue, -1);   // callback not fired
}

// ============================================================================
// JSON serialization
// ============================================================================

TEST(NumericParam, ToJsonRoundTrip)
{
    settings::NumericParam<int> original("nk", "NT", "ND");
    const auto                  limits = std::pair(0, 100);
    original.setLimits(limits.first, limits.second);
    const auto value  = 50;
    auto       result = original.set(value);
    ASSERT_TRUE(result.has_value());

    auto json = original.toJson();

    settings::NumericParam<int> restored("", "", "");
    settings::NumericParam<int>::fromJson(json, restored);

    EXPECT_EQ(restored.getKey(), "nk");
    EXPECT_EQ(restored.get(), 50);
    ASSERT_TRUE(restored.getMinValue().has_value());
    EXPECT_EQ(restored.getMinValue().value(), 0);
    ASSERT_TRUE(restored.getMaxValue().has_value());
    EXPECT_EQ(restored.getMaxValue().value(), 100);
}

TEST(NumericParam, ToJsonRoundTripDouble)
{
    settings::NumericParam<double> original("dk", "DT", "DD");
    original.setPrecision(2);
    const auto piValue = 3.14;
    auto       result  = original.set(piValue);
    ASSERT_TRUE(result.has_value());

    auto json = original.toJson();

    settings::NumericParam<double> restored("", "", "");
    settings::NumericParam<double>::fromJson(json, restored);

    EXPECT_NEAR(restored.get(), piValue, 1e-9);
    ASSERT_TRUE(restored.getPrecision().has_value());
    EXPECT_EQ(restored.getPrecision().value(), static_cast<std::size_t>(2));
}

TEST(NumericParam, ToJsonContainsRangeKeys)
{
    settings::NumericParam<int> param("k", "T", "D");
    const auto                  limits = std::pair(1, 9);
    param.setLimits(limits.first, limits.second);
    auto json = param.toJson();
    EXPECT_TRUE(json.contains("minValue"));
    EXPECT_TRUE(json.contains("maxValue"));
    EXPECT_TRUE(json.contains("precision"));
}
