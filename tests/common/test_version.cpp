// tests/utils/test_version.cpp
//
// GoogleTest-based tests for common::SemVer.
//
// Coverage:
//  - Construction from (major, minor, patch) numbers
//  - Construction from a valid version string
//  - Construction from an invalid version string produces an invalid version
//  - getMajor / getMinor / getPatch accessors
//  - toString round-trip
//  - getInvalidVersion returns an invalid (equal-to-invalid) instance
//  - operator==: equal versions compare equal; different versions compare not
//  equal
//  - operator==: two invalid versions compare equal
//  - Build metadata suffix (+...) is accepted and ignored

#include <gtest/gtest.h>

#include <string>

#include "common/version.hpp"

TEST(UtilsSemVer, ConstructFromNumbersAccessors)
{
    const common::SemVer version(1, 2, 3);
    EXPECT_EQ(version.getMajor(), 1U);
    EXPECT_EQ(version.getMinor(), 2U);
    EXPECT_EQ(version.getPatch(), 3U);
}

TEST(UtilsSemVer, ConstructFromValidStringAccessors)
{
    const common::SemVer version(std::string{"4.5.6"});
    EXPECT_EQ(version.getMajor(), 4U);
    EXPECT_EQ(version.getMinor(), 5U);
    EXPECT_EQ(version.getPatch(), 6U);
}

TEST(UtilsSemVer, ToStringRoundTrip)
{
    const common::SemVer version(1, 0, 0);
    EXPECT_EQ(version.toString(), "1.0.0");
}

TEST(UtilsSemVer, ToStringMultiDigit)
{
    const common::SemVer version(10, 20, 30);
    EXPECT_EQ(version.toString(), "10.20.30");
}

TEST(UtilsSemVer, ParsedToStringMatchesInput)
{
    const std::string    input{"3.14.159"};
    const common::SemVer version(input);
    EXPECT_EQ(version.toString(), "3.14.159");
}

TEST(UtilsSemVer, EqualVersionsCompareEqual)
{
    const common::SemVer versionA(2, 3, 4);
    const common::SemVer versionB(2, 3, 4);
    EXPECT_EQ(versionA, versionB);
}

TEST(UtilsSemVer, DifferentMajorNotEqual)
{
    const common::SemVer versionA(1, 0, 0);
    const common::SemVer versionB(2, 0, 0);
    EXPECT_NE(versionA, versionB);
}

TEST(UtilsSemVer, DifferentMinorNotEqual)
{
    const common::SemVer versionA(1, 0, 0);
    const common::SemVer versionB(1, 1, 0);
    EXPECT_NE(versionA, versionB);
}

TEST(UtilsSemVer, DifferentPatchNotEqual)
{
    const common::SemVer versionA(1, 0, 0);
    const common::SemVer versionB(1, 0, 1);
    EXPECT_NE(versionA, versionB);
}

TEST(UtilsSemVer, InvalidVersionStringProducesInvalid)
{
    const common::SemVer version(std::string{"not-a-version"});
    EXPECT_EQ(version, common::SemVer::getInvalidVersion());
}

TEST(UtilsSemVer, TwoInvalidVersionsCompareEqual)
{
    const common::SemVer versionA(std::string{"bad"});
    const common::SemVer versionB(std::string{"also-bad"});
    EXPECT_EQ(versionA, versionB);
}

TEST(UtilsSemVer, InvalidNotEqualToValid)
{
    const common::SemVer invalid(std::string{"nope"});
    const common::SemVer valid(1, 0, 0);
    EXPECT_NE(invalid, valid);
}

TEST(UtilsSemVer, BuildMetadataSuffixAccepted)
{
    const common::SemVer version(std::string{"1.2.3+build.42"});
    EXPECT_EQ(version.getMajor(), 1U);
    EXPECT_EQ(version.getMinor(), 2U);
    EXPECT_EQ(version.getPatch(), 3U);
}

TEST(UtilsSemVer, ZeroVersionParsesCorrectly)
{
    const common::SemVer version(std::string{"0.0.0"});
    EXPECT_EQ(version.getMajor(), 0U);
    EXPECT_EQ(version.getMinor(), 0U);
    EXPECT_EQ(version.getPatch(), 0U);
    EXPECT_EQ(version, common::SemVer(0, 0, 0));
}
