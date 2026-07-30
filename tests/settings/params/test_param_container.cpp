#include <gtest/gtest.h>

#include "settings/params/param_container.hpp"

TEST(ParamContainer, StoresKey)
{
    settings::ParamContainer container("myKey", "My Title", "My Desc");
    EXPECT_EQ(container.getKey(), "myKey");
}

TEST(ParamContainer, StoresTitle)
{
    settings::ParamContainer container("myKey", "My Title", "My Desc");
    EXPECT_EQ(container.getTitle(), "My Title");
}

TEST(ParamContainer, StoresDescription)
{
    settings::ParamContainer container("myKey", "My Title", "My Desc");
    EXPECT_EQ(container.getDescription(), "My Desc");
}

TEST(ParamContainer, EmptyStringsAllowed)
{
    settings::ParamContainer container("", "", "");
    EXPECT_EQ(container.getKey(), "");
    EXPECT_EQ(container.getTitle(), "");
    EXPECT_EQ(container.getDescription(), "");
}
