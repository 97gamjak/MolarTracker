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
    auto   tup   = std::make_tuple(1, 2.0, std::string("three"));
    int    count = 0;
    auto fn  = [&](auto& /*elem*/) { count++; };
    settings::forEachParam(tup, fn);
    EXPECT_EQ(count, 3);
}

TEST(ParamUtils, ForEachParamPassesCorrectElements)
{
    auto tup = std::make_tuple(10, 20, 30);
    int  sum = 0;
    settings::forEachParam(tup, [&](auto& v) { sum += v; });
    EXPECT_EQ(sum, 60);
}

TEST(ParamUtils, ForEachParamWorksOnSingleElementTuple)
{
    auto tup   = std::make_tuple(42);
    int  count = 0;
    settings::forEachParam(tup, [&](auto& /*v*/) { count++; });
    EXPECT_EQ(count, 1);
}

// ============================================================================
// paramsToJson / paramsFromJson
// ============================================================================

TEST(ParamUtils, ParamsToJsonCreatesKeyedJson)
{
    settings::ParamCore<int>         p1("k1", "T1", "D1");
    settings::ParamCore<std::string> p2("k2", "T2", "D2");
    p1.set(7);
    p2.set(std::string("hello"));

    auto tup  = std::forward_as_tuple(p1, p2);
    auto json = settings::paramsToJson(tup);

    EXPECT_TRUE(json.contains("k1"));
    EXPECT_TRUE(json.contains("k2"));
}

TEST(ParamUtils, ParamsFromJsonRestoresValues)
{
    settings::ParamCore<int>         p1("k1", "T1", "D1");
    settings::ParamCore<std::string> p2("k2", "T2", "D2");
    p1.set(55);
    p2.set(std::string("world"));

    auto tup  = std::forward_as_tuple(p1, p2);
    auto json = settings::paramsToJson(tup);

    settings::ParamCore<int>         r1("k1", "T1", "D1");
    settings::ParamCore<std::string> r2("k2", "T2", "D2");
    auto                             restoreTup = std::forward_as_tuple(r1, r2);
    settings::paramsFromJson(restoreTup, json);

    EXPECT_EQ(r1.get(), 55);
    EXPECT_EQ(r2.get(), std::string("world"));
}

TEST(ParamUtils, ParamsFromJsonSkipsMissingKeys)
{
    settings::ParamCore<int> p("present", "T", "D");
    p.set(10);

    auto tup  = std::forward_as_tuple(p);
    auto json = settings::paramsToJson(tup);

    // Param with key not present in JSON should be left untouched
    settings::ParamCore<int> r("absent", "T", "D");
    r.set(99);
    auto restoreTup = std::forward_as_tuple(r);
    settings::paramsFromJson(restoreTup, json);

    // "absent" key not in JSON, so r keeps its original value
    EXPECT_EQ(r.get(), 99);
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
    EXPECT_TRUE((settings::is_numeric_param<settings::NumericParam<int>>));
}

TEST(ParamUtils, IsNumericParamTrueForDoubleParam)
{
    EXPECT_TRUE((settings::is_numeric_param<settings::NumericParam<double>>));
}

TEST(ParamUtils, IsNumericParamFalseForBoolParam)
{
    EXPECT_FALSE(settings::is_numeric_param<settings::BoolParam>);
}

TEST(ParamUtils, IsEnumParamTrueForEnumParam)
{
    EXPECT_TRUE(
        (settings::is_enum_param<settings::EnumParam<LogLevel>>)
    );
}

TEST(ParamUtils, IsEnumParamFalseForStringParam)
{
    EXPECT_FALSE(settings::is_enum_param<settings::StringParam>);
}

TEST(ParamUtils, IsNumericVecParamTrueForVec2)
{
    EXPECT_TRUE(
        (settings::is_numeric_vec_param<settings::NumericVecParam<int, 2>>)
    );
}

TEST(ParamUtils, IsNumericVecParamTrueForVec3)
{
    EXPECT_TRUE(
        (settings::is_numeric_vec_param<settings::NumericVecParam<double, 3>>)
    );
}

TEST(ParamUtils, IsNumericVecParamFalseForNumericParam)
{
    EXPECT_FALSE(
        (settings::is_numeric_vec_param<settings::NumericParam<int>>)
    );
}
