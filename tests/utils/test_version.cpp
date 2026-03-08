// tests/utils/test_version.cpp
//
// GoogleTest-based tests for utils::SemVer.
//
// Coverage:
//  - Construction from (major, minor, patch) numbers
//  - Construction from a valid version string
//  - Construction from an invalid version string produces an invalid version
//  - getMajor / getMinor / getPatch accessors
//  - toString round-trip
//  - getInvalidVersion returns an invalid (equal-to-invalid) instance
//  - operator==: equal versions compare equal; different versions compare not equal
//  - operator==: two invalid versions compare equal
//  - Build metadata suffix (+...) is accepted and ignored

#include <gtest/gtest.h>

#include <string>

#include "utils/version.hpp"

TEST(UtilsSemVer, ConstructFromNumbersAccessors)
{
    const utils::SemVer v(1, 2, 3);
    EXPECT_EQ(v.getMajor(), 1u);
    EXPECT_EQ(v.getMinor(), 2u);
    EXPECT_EQ(v.getPatch(), 3u);
}

TEST(UtilsSemVer, ConstructFromValidStringAccessors)
{
    const utils::SemVer v(std::string{"4.5.6"});
    EXPECT_EQ(v.getMajor(), 4u);
    EXPECT_EQ(v.getMinor(), 5u);
    EXPECT_EQ(v.getPatch(), 6u);
}

TEST(UtilsSemVer, ToStringRoundTrip)
{
    const utils::SemVer v(1, 0, 0);
    EXPECT_EQ(v.toString(), "1.0.0");
}

TEST(UtilsSemVer, ToStringMultiDigit)
{
    const utils::SemVer v(10, 20, 30);
    EXPECT_EQ(v.toString(), "10.20.30");
}

TEST(UtilsSemVer, ParsedToStringMatchesInput)
{
    const std::string   input{"3.14.159"};
    const utils::SemVer v(input);
    EXPECT_EQ(v.toString(), "3.14.159");
}

TEST(UtilsSemVer, EqualVersionsCompareEqual)
{
    const utils::SemVer a(2, 3, 4);
    const utils::SemVer b(2, 3, 4);
    EXPECT_EQ(a, b);
}

TEST(UtilsSemVer, DifferentMajorNotEqual)
{
    const utils::SemVer a(1, 0, 0);
    const utils::SemVer b(2, 0, 0);
    EXPECT_NE(a, b);
}

TEST(UtilsSemVer, DifferentMinorNotEqual)
{
    const utils::SemVer a(1, 0, 0);
    const utils::SemVer b(1, 1, 0);
    EXPECT_NE(a, b);
}

TEST(UtilsSemVer, DifferentPatchNotEqual)
{
    const utils::SemVer a(1, 0, 0);
    const utils::SemVer b(1, 0, 1);
    EXPECT_NE(a, b);
}

TEST(UtilsSemVer, InvalidVersionStringProducesInvalid)
{
    const utils::SemVer v(std::string{"not-a-version"});
    EXPECT_EQ(v, utils::SemVer::getInvalidVersion());
}

TEST(UtilsSemVer, TwoInvalidVersionsCompareEqual)
{
    const utils::SemVer a(std::string{"bad"});
    const utils::SemVer b(std::string{"also-bad"});
    EXPECT_EQ(a, b);
}

TEST(UtilsSemVer, InvalidNotEqualToValid)
{
    const utils::SemVer invalid(std::string{"nope"});
    const utils::SemVer valid(1, 0, 0);
    EXPECT_NE(invalid, valid);
}

TEST(UtilsSemVer, BuildMetadataSuffixAccepted)
{
    const utils::SemVer v(std::string{"1.2.3+build.42"});
    EXPECT_EQ(v.getMajor(), 1u);
    EXPECT_EQ(v.getMinor(), 2u);
    EXPECT_EQ(v.getPatch(), 3u);
}

TEST(UtilsSemVer, ZeroVersionParsesCorrectly)
{
    const utils::SemVer v(std::string{"0.0.0"});
    EXPECT_EQ(v.getMajor(), 0u);
    EXPECT_EQ(v.getMinor(), 0u);
    EXPECT_EQ(v.getPatch(), 0u);
    EXPECT_EQ(v, utils::SemVer(0, 0, 0));
}
