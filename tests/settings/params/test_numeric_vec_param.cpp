#include <gtest/gtest.h>

#include <cstddef>
#include <string>
#include <vector>

#include "settings/params/numeric_vec_param.hpp"
#include "settings/params/param_error.hpp"

// ============================================================================
// Construction
// ============================================================================

TEST(NumericVecParam, ConstructorCreatesNParams)
{
    settings::NumericVecParam<int, 3> vec("pos", "Position", "3D position");
    EXPECT_EQ(vec.size, static_cast<std::size_t>(3));
}

TEST(NumericVecParam, ConstructorSetsKeyPrefix)
{
    settings::NumericVecParam<int, 2> vec("coord", "Coord", "Coordinate");
    EXPECT_EQ(vec.getKey(), "coord");
}

TEST(NumericVecParam, ConstructorIndexSuffixesParamKeys)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "Vec");
    EXPECT_EQ(vec.getParam(0).getKey(), "v0");
    EXPECT_EQ(vec.getParam(1).getKey(), "v1");
}

TEST(NumericVecParam, ConstructorSetsTitle)
{
    settings::NumericVecParam<int, 2> vec("k", "MyTitle", "D");
    EXPECT_EQ(vec.getTitle(), "MyTitle");
}

TEST(NumericVecParam, ConstructorSetsDescription)
{
    settings::NumericVecParam<int, 2> vec("k", "T", "MyDesc");
    EXPECT_EQ(vec.getDescription(), "MyDesc");
}

// ============================================================================
// set / get
// ============================================================================

TEST(NumericVecParam, SetAndGetByIndex)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    const auto                        value  = 10;
    auto                              result = vec.set(0, value);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(vec.get(0), value);
}

TEST(NumericVecParam, SetMultipleIndices)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    ASSERT_TRUE(vec.set(0, 1).has_value());
    ASSERT_TRUE(vec.set(1, 2).has_value());
    ASSERT_TRUE(vec.set(2, 3).has_value());
    EXPECT_EQ(vec.get(0), 1);
    EXPECT_EQ(vec.get(1), 2);
    EXPECT_EQ(vec.get(2), 3);
}

TEST(NumericVecParam, SetOutOfRangeIndexReturnsError)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    const auto                        paramValue = 99;

    auto result = vec.set(2, paramValue);   // index == N
    EXPECT_FALSE(result.has_value());
    EXPECT_FALSE(result.error().getMessage().empty());
}

TEST(NumericVecParam, GetOutOfRangeIndexThrows)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    ASSERT_TRUE(vec.set(0, 1).has_value());
    ASSERT_TRUE(vec.set(1, 2).has_value());
    EXPECT_THROW((void) vec.get(2), settings::ParamException);
}

TEST(NumericVecParam, GetParamOutOfRangeThrows)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    EXPECT_THROW((void) vec.getParam(3), settings::ParamException);
}

// ============================================================================
// Default values
// ============================================================================

TEST(NumericVecParam, SetDefaultsVectorAppliesDefaults)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    const auto                        value1 = 10;
    const auto                        value2 = 20;
    const auto                        value3 = 30;

    vec.setDefaults(std::vector<int>{value1, value2, value3});

    EXPECT_EQ(vec.get(0), value1);
    EXPECT_EQ(vec.get(1), value2);
    EXPECT_EQ(vec.get(2), value3);
}

TEST(NumericVecParam, SetDefaultsVectorWrongSizeThrows)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    EXPECT_THROW(
        vec.setDefaults(std::vector<int>{1, 2}),
        settings::ParamException
    );
}

TEST(NumericVecParam, SetDefaultsPairForSize2)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    const auto                        value1 = 5;
    const auto                        value2 = 7;

    vec.setDefaults(std::pair<int, int>{value1, value2});

    EXPECT_EQ(vec.get(0), value1);
    EXPECT_EQ(vec.get(1), value2);
}

// ============================================================================
// Min values
// ============================================================================

TEST(NumericVecParam, SetMinValuesVectorApplied)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    vec.setMinValues(std::vector<int>{0, 0});
    // attempt to set below min
    EXPECT_FALSE(vec.set(0, -1).has_value());
    EXPECT_FALSE(vec.set(1, -1).has_value());
}

TEST(NumericVecParam, SetMinValuesVectorWrongSizeThrows)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    EXPECT_THROW(
        vec.setMinValues(std::vector<int>{1}),
        settings::ParamException
    );
}

TEST(NumericVecParam, SetMinValuesPairForSize2)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    const auto                        value1 = 10;
    const auto                        value2 = 20;

    vec.setMinValues(std::pair<int, int>{value1, value2});

    EXPECT_FALSE(vec.set(0, 5).has_value());
    EXPECT_FALSE(vec.set(1, 15).has_value());
    EXPECT_TRUE(vec.set(0, value1).has_value());
    EXPECT_TRUE(vec.set(1, value2).has_value());
}

// ============================================================================
// Convenience accessors x / y / z
// ============================================================================

TEST(NumericVecParam, XAccessorReturnsFirstElement)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    const auto                        value1 = 11;
    const auto                        value2 = 22;
    vec.setDefaults(std::vector<int>{value1, value2});
    EXPECT_EQ(vec.x(), value1);
}

TEST(NumericVecParam, YAccessorReturnsSecondElement)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    const auto                        value1 = 11;
    const auto                        value2 = 22;
    vec.setDefaults(std::vector<int>{value1, value2});
    EXPECT_EQ(vec.y(), value2);
}

TEST(NumericVecParam, ZAccessorReturnsThirdElement)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    vec.setDefaults(std::vector<int>{1, 2, 3});
    EXPECT_EQ(vec.z(), 3);
}

// ============================================================================
// isDirty / commit
// ============================================================================

TEST(NumericVecParam, IsDirtyFalseOnFreshParam)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    EXPECT_FALSE(vec.isDirty());
}

TEST(NumericVecParam, IsDirtyTrueAfterSet)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    ASSERT_TRUE(vec.set(0, 5).has_value());
    EXPECT_TRUE(vec.isDirty());
}

TEST(NumericVecParam, CommitClearsDirty)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    ASSERT_TRUE(vec.set(0, 5).has_value());
    vec.commit();
    EXPECT_FALSE(vec.isDirty());
}

TEST(NumericVecParam, IsDirtyTrueWhenAnyElementChanged)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    vec.setDefaults(std::vector<int>{0, 0, 0});
    vec.commit();
    ASSERT_TRUE(vec.set(2, 99).has_value());
    EXPECT_TRUE(vec.isDirty());
}

TEST(NumericVecParam, CommitClearsDirtyForAllElements)
{
    settings::NumericVecParam<int, 3> vec("v", "V", "D");
    ASSERT_TRUE(vec.set(0, 1).has_value());
    ASSERT_TRUE(vec.set(1, 2).has_value());
    ASSERT_TRUE(vec.set(2, 3).has_value());
    vec.commit();
    EXPECT_FALSE(vec.isDirty());
}

TEST(NumericVecParam, ResetToDefaultRestoresAllElements)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    vec.setDefaults(std::vector<int>{1, 2});
    ASSERT_TRUE(vec.set(0, 10).has_value());
    ASSERT_TRUE(vec.set(1, 20).has_value());

    vec.resetToDefault();

    EXPECT_EQ(vec.get(0), 1);
    EXPECT_EQ(vec.get(1), 2);
}

// ============================================================================
// toString
// ============================================================================

TEST(NumericVecParam, ToStringContainsValues)
{
    settings::NumericVecParam<int, 2> vec("v", "V", "D");
    const auto                        value1 = 4;
    const auto                        value2 = 8;
    vec.setDefaults(std::vector<int>{value1, value2});
    const auto str = vec.toString();
    EXPECT_NE(str.find(std::to_string(value1)), std::string::npos);
    EXPECT_NE(str.find(std::to_string(value2)), std::string::npos);
}

// ============================================================================
// JSON serialization
// ============================================================================

TEST(NumericVecParam, ToJsonContainsParamsArray)
{
    settings::NumericVecParam<int, 2> vec("vk", "VT", "VD");
    auto                              json = vec.toJson();
    EXPECT_TRUE(json.contains("key"));
    EXPECT_TRUE(json.contains("title"));
    EXPECT_TRUE(json.contains("description"));
    EXPECT_TRUE(json.contains("params"));
    EXPECT_TRUE(json["params"].is_array());
    EXPECT_EQ(json["params"].size(), static_cast<std::size_t>(2));
}

TEST(NumericVecParam, ToJsonRoundTrip)
{
    settings::NumericVecParam<int, 3> original("pos", "Pos", "Position");
    const auto                        value1 = 10;
    const auto                        value2 = 20;
    const auto                        value3 = 30;
    original.setDefaults(std::vector<int>{value1, value2, value3});

    auto json = original.toJson();

    settings::NumericVecParam<int, 3> restored("pos", "Pos", "Position");
    settings::NumericVecParam<int, 3>::fromJson(json, restored);

    EXPECT_EQ(restored.getKey(), "pos");
    EXPECT_EQ(restored.get(0), value1);
    EXPECT_EQ(restored.get(1), value2);
    EXPECT_EQ(restored.get(2), value3);
}

TEST(NumericVecParam, FromJsonWrongArraySizeThrows)
{
    using Vec3 = settings::NumericVecParam<int, 3>;

    Vec3 vec("v", "V", "D");
    auto json = vec.toJson();

    // corrupt the params array to have wrong size
    json["params"] =
        nlohmann::json::array({json["params"][0], json["params"][1]});

    Vec3 target("v", "V", "D");
    EXPECT_THROW(Vec3::fromJson(json, target), settings::ParamException);
}
