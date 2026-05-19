#include <gtest/gtest.h>

#include <cmath>
#include <cstddef>
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
    auto                        result = param.set(5);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), 5);
}

TEST(NumericParam, IsDirtyFalseBeforeSet)
{
    settings::NumericParam<int> param("k", "T", "D");
    EXPECT_FALSE(param.isDirty());
}

TEST(NumericParam, IsDirtyTrueAfterSet)
{
    settings::NumericParam<int> param("k", "T", "D");
    auto                        r = param.set(10);
    ASSERT_TRUE(r.has_value());
    EXPECT_TRUE(param.isDirty());
}

TEST(NumericParam, CommitClearsDirty)
{
    settings::NumericParam<int> param("k", "T", "D");
    auto                        r = param.set(10);
    ASSERT_TRUE(r.has_value());
    param.commit();
    EXPECT_FALSE(param.isDirty());
}

// ============================================================================
// Default value
// ============================================================================

TEST(NumericParam, SetDefaultAndGetDefault)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setDefault(std::optional<int>(100));
    ASSERT_TRUE(param.getDefault().has_value());
    EXPECT_EQ(param.getDefault().value(), 100);
}

// ============================================================================
// Range limits
// ============================================================================

TEST(NumericParam, SetLimitsStoresMinMax)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(0, 100);
    ASSERT_TRUE(param.getMinValue().has_value());
    ASSERT_TRUE(param.getMaxValue().has_value());
    EXPECT_EQ(param.getMinValue().value(), 0);
    EXPECT_EQ(param.getMaxValue().value(), 100);
}

TEST(NumericParam, SetMinValueStoresMin)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setMinValue(5);
    ASSERT_TRUE(param.getMinValue().has_value());
    EXPECT_EQ(param.getMinValue().value(), 5);
}

TEST(NumericParam, SetMaxValueStoresMax)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setMaxValue(200);
    ASSERT_TRUE(param.getMaxValue().has_value());
    EXPECT_EQ(param.getMaxValue().value(), 200);
}

TEST(NumericParam, SetBelowMinReturnsError)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(10, 100);
    auto result = param.set(9);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().getMessage().empty());
}

TEST(NumericParam, SetAboveMaxReturnsError)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(0, 50);
    auto result = param.set(51);
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().getMessage().empty());
}

TEST(NumericParam, SetAtMinBoundarySucceeds)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(0, 100);
    auto result = param.set(0);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), 0);
}

TEST(NumericParam, SetAtMaxBoundarySucceeds)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(0, 100);
    auto result = param.set(100);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(param.get(), 100);
}

TEST(NumericParam, SetWithOnlyMinAllowsAboveMin)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setMinValue(5);
    EXPECT_TRUE(param.set(1000).has_value());
}

TEST(NumericParam, SetWithOnlyMinRejectsBelow)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setMinValue(5);
    EXPECT_FALSE(param.set(4).has_value());
}

TEST(NumericParam, SetWithOnlyMaxAllowsBelowMax)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setMaxValue(100);
    EXPECT_TRUE(param.set(-9999).has_value());
}

TEST(NumericParam, SetWithOnlyMaxRejectsAbove)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setMaxValue(100);
    EXPECT_FALSE(param.set(101).has_value());
}

// ============================================================================
// Precision (floating-point)
// ============================================================================

TEST(NumericParam, SetPrecisionRoundsDouble)
{
    settings::NumericParam<double> param("k", "T", "D");
    param.setPrecision(2);   // 2 decimal places
    auto result = param.set(3.14159);
    ASSERT_TRUE(result.has_value());
    EXPECT_NEAR(param.get(), 3.14, 1e-9);
}

TEST(NumericParam, SetPrecisionRoundsUp)
{
    settings::NumericParam<double> param("k", "T", "D");
    param.setPrecision(1);
    auto result = param.set(1.95);
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
    param.setPrecision(2);
    param.setLimits(0.0, 10.0);
    auto result = param.set(2.005);
    EXPECT_TRUE(result.has_value());
}

// ============================================================================
// subscribe (via ParamMixin — raw function pointer)
// ============================================================================

namespace
{
    int  s_numericIntValue = 0;
    void onNumericIntChanged(const int& v)
    {
        s_numericIntValue = v;
    }
}   // namespace

TEST(NumericParam, SubscribeCallbackFiredOnValidSet)
{
    s_numericIntValue = -1;
    settings::NumericParam<int> param("k", "T", "D");
    auto conn = param.subscribe(onNumericIntChanged, nullptr);

    auto r = param.set(55);
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(s_numericIntValue, 55);
}

TEST(NumericParam, SubscribeCallbackNotFiredOnOutOfRangeSet)
{
    s_numericIntValue = -1;
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(0, 10);
    auto conn = param.subscribe(onNumericIntChanged, nullptr);

    auto r = param.set(99);   // out of range
    EXPECT_FALSE(r.has_value());
    EXPECT_EQ(s_numericIntValue, -1);   // callback not fired
}

// ============================================================================
// JSON serialisation
// ============================================================================

TEST(NumericParam, ToJsonRoundTrip)
{
    settings::NumericParam<int> original("nk", "NT", "ND");
    original.setLimits(0, 100);
    auto r = original.set(50);
    ASSERT_TRUE(r.has_value());

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
    auto r = original.set(3.14);
    ASSERT_TRUE(r.has_value());

    auto json = original.toJson();

    settings::NumericParam<double> restored("", "", "");
    settings::NumericParam<double>::fromJson(json, restored);

    EXPECT_NEAR(restored.get(), 3.14, 1e-9);
    ASSERT_TRUE(restored.getPrecision().has_value());
    EXPECT_EQ(restored.getPrecision().value(), static_cast<std::size_t>(2));
}

TEST(NumericParam, ToJsonContainsRangeKeys)
{
    settings::NumericParam<int> param("k", "T", "D");
    param.setLimits(1, 9);
    auto json = param.toJson();
    EXPECT_TRUE(json.contains("minValue"));
    EXPECT_TRUE(json.contains("maxValue"));
    EXPECT_TRUE(json.contains("precision"));
}
