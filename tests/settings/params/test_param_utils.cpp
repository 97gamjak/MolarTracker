#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <string>
#include <tuple>

#include "config/logging_base.hpp"
#include "settings/params/param_utils.hpp"

// ============================================================================
// forEachParam
// ============================================================================

TEST(ParamUtils, ForEachParamCallsFuncForEveryElement)
{
    constexpr auto tuple1 = 1;
    constexpr auto tuple2 = 2.0;
    const auto     tuple3 = std::string("three");
    auto           tup    = std::make_tuple(tuple1, tuple2, tuple3);
    int            count  = 0;
    auto           func   = [&](auto& /*elem*/) { count++; };
    settings::forEachParam(tup, func);
    EXPECT_EQ(count, 3);
}

TEST(ParamUtils, ForEachParamPassesCorrectElements)
{
    constexpr auto tuple1 = 10;
    constexpr auto tuple2 = 20;
    constexpr auto tuple3 = 30;
    auto           tup    = std::make_tuple(tuple1, tuple2, tuple3);
    int            sum    = 0;
    settings::forEachParam(tup, [&](auto& value) { sum += value; });
    EXPECT_EQ(sum, 60);
}

TEST(ParamUtils, ForEachParamWorksOnSingleElementTuple)
{
    constexpr auto tuple1 = 42;
    auto           tup    = std::make_tuple(tuple1);
    int            count  = 0;
    settings::forEachParam(tup, [&](auto& /*v*/) { count++; });
    EXPECT_EQ(count, 1);
}

// ============================================================================
// paramsToJson / paramsFromJson
// ============================================================================

TEST(ParamUtils, ParamsToJsonCreatesKeyedJson)
{
    constexpr auto                   value1 = "k1";
    constexpr auto                   value2 = "k2";
    settings::ParamCore<int>         param1(value1, "T1", "D1");
    settings::ParamCore<std::string> param2(value2, "T2", "D2");

    const auto paramValue = 7;
    param1.set(paramValue);
    param2.set(std::string("hello"));

    auto tup  = std::forward_as_tuple(param1, param2);
    auto json = settings::paramsToJson(tup);

    EXPECT_TRUE(json.contains(value1));
    EXPECT_TRUE(json.contains(value2));
}

TEST(ParamUtils, ParamsFromJsonRestoresValues)
{
    constexpr auto param1Key   = "k1";
    constexpr auto param1Title = "T1";
    constexpr auto param1Desc  = "D1";
    constexpr auto param1Value = 55;

    constexpr auto param2Key   = "k2";
    constexpr auto param2Title = "T2";
    constexpr auto param2Desc  = "D2";
    const auto     param2Value = std::string("world");

    settings::ParamCore<int>         param1(param1Key, param1Title, param1Desc);
    settings::ParamCore<std::string> param2(param2Key, param2Title, param2Desc);

    param1.set(param1Value);
    param2.set(param2Value);

    auto tup  = std::forward_as_tuple(param1, param2);
    auto json = settings::paramsToJson(tup);

    settings::ParamCore<int> param1Copy(param1Key, param1Title, param1Desc);
    settings::ParamCore<std::string> param2Copy(
        param2Key,
        param2Title,
        param2Desc
    );
    auto restoreTup = std::forward_as_tuple(param1Copy, param2Copy);
    settings::paramsFromJson(restoreTup, json);

    EXPECT_EQ(param1Copy.get(), param1Value);
    EXPECT_EQ(param2Copy.get(), param2Value);
}

TEST(ParamUtils, ParamsFromJsonSkipsMissingKeys)
{
    constexpr auto param1Key   = "k1";
    constexpr auto param1Title = "T1";
    constexpr auto param1Desc  = "D1";
    constexpr auto param1Value = 10;

    settings::ParamCore<int> param(param1Key, param1Title, param1Desc);
    param.set(param1Value);

    auto tup  = std::forward_as_tuple(param);
    auto json = settings::paramsToJson(tup);

    constexpr auto param2Key   = "absent";
    constexpr auto param2Title = "T";
    constexpr auto param2Desc  = "D";
    constexpr auto param2Value = 99;

    // Param with key not present in JSON should be left untouched
    settings::ParamCore<int> param2(param2Key, param2Title, param2Desc);
    param2.set(param2Value);
    auto restoreTup = std::forward_as_tuple(param2);
    settings::paramsFromJson(restoreTup, json);

    // "absent" key not in JSON, so param2 keeps its original value
    EXPECT_EQ(param2.get(), param2Value);
}

// ============================================================================
// Type traits
// ============================================================================

TEST(ParamUtils, IsBoolParamTrueForBoolParam)
{
    EXPECT_TRUE(settings::is_bool_param<settings::BoolParam>);
}

TEST(ParamUtils, IsBoolParamFalseForStringParam)
{
    EXPECT_FALSE(settings::is_bool_param<settings::StringParam>);
}

TEST(ParamUtils, IsStringParamTrueForStringParam)
{
    EXPECT_TRUE(settings::is_string_param<settings::StringParam>);
}

TEST(ParamUtils, IsStringParamFalseForBoolParam)
{
    EXPECT_FALSE(settings::is_string_param<settings::BoolParam>);
}

TEST(ParamUtils, IsNumericParamTrueForIntParam)
{
    EXPECT_TRUE((settings::is_numeric_param<settings::NumericParam<int>>) );
}

TEST(ParamUtils, IsNumericParamTrueForDoubleParam)
{
    EXPECT_TRUE((settings::is_numeric_param<settings::NumericParam<double>>) );
}

TEST(ParamUtils, IsNumericParamFalseForBoolParam)
{
    EXPECT_FALSE(settings::is_numeric_param<settings::BoolParam>);
}

TEST(ParamUtils, IsEnumParamTrueForEnumParam)
{
    EXPECT_TRUE((settings::is_enum_param<settings::EnumParam<LogLevel>>) );
}

TEST(ParamUtils, IsEnumParamFalseForStringParam)
{
    EXPECT_FALSE(settings::is_enum_param<settings::StringParam>);
}

TEST(ParamUtils, IsNumericVecParamTrueForVec2)
{
    EXPECT_TRUE((
        settings::is_numeric_vec_param<settings::NumericVecParam<int, 2>>
    ) );
}

TEST(ParamUtils, IsNumericVecParamTrueForVec3)
{
    EXPECT_TRUE((
        settings::is_numeric_vec_param<settings::NumericVecParam<double, 3>>
    ) );
}

TEST(ParamUtils, IsNumericVecParamFalseForNumericParam)
{
    EXPECT_FALSE((settings::is_numeric_vec_param<settings::NumericParam<int>>
    ) );
}
