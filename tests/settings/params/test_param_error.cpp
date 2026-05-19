#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

#include "settings/params/param_error.hpp"

TEST(ParamError, StoresAndReturnsMessage)
{
    settings::ParamError err("some error message");
    EXPECT_EQ(err.getMessage(), "some error message");
}

TEST(ParamError, EmptyMessageAllowed)
{
    settings::ParamError err("");
    EXPECT_EQ(err.getMessage(), "");
}

TEST(ParamException, WhatContainsMessage)
{
    settings::ParamException ex("param went wrong");
    const std::string        what = ex.what();
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
        caught        = true;
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
        caught        = true;
        const auto msg = std::string(e.what());
        EXPECT_NE(msg.find("specific type"), std::string::npos);
    }
    EXPECT_TRUE(caught);
}
