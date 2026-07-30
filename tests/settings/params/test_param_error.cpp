#include <gtest/gtest.h>

#include <string>

#include "settings/params/param_error.hpp"

TEST(ParamException, WhatContainsMessage)
{
    settings::ParamException exception("param went wrong");
    const std::string        what = exception.what();
    EXPECT_NE(what.find("param went wrong"), std::string::npos);
}

TEST(ParamException, CanBeCaughtAsStdException)
{
    bool caught = false;
    try
    {
        throw settings::ParamException("thrown exception");
    }
    catch (const std::exception& e)
    {
        caught         = true;
        const auto msg = std::string(e.what());
        EXPECT_NE(msg.find("thrown exception"), std::string::npos);
    }
    EXPECT_TRUE(caught);
}

TEST(ParamException, CanBeCaughtAsParamException)
{
    bool caught = false;
    try
    {
        throw settings::ParamException("specific type");
    }
    catch (const settings::ParamException& e)
    {
        caught         = true;
        const auto msg = std::string(e.what());
        EXPECT_NE(msg.find("specific type"), std::string::npos);
    }
    EXPECT_TRUE(caught);
}
