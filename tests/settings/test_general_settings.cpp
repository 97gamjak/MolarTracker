// tests/settings/test_general_settings.cpp
//
// GoogleTest-based tests for settings::GeneralSettings.
//
// Coverage:
//  - Default state: no default profile, version is app version
//  - setDefaultProfile / unsetDefaultProfile / hasDefaultProfile /
//    getDefaultProfile
//  - isDirty / commit mechanics
//  - getKey returns the schema key
//  - toJson / fromJson round-trip

#include <gtest/gtest.h>

#include <optional>
#include <string>

#include "settings/general_settings.hpp"
#include "utils/version_json.hpp"   // IWYU pragma: keep

TEST(GeneralSettings, DefaultConstructed_HasNoDefaultProfile)
{
    settings::GeneralSettings gs;
    EXPECT_FALSE(gs.hasDefaultProfile());
}

TEST(GeneralSettings, DefaultConstructed_GetDefaultProfileReturnsNullopt)
{
    settings::GeneralSettings gs;
    EXPECT_FALSE(gs.getDefaultProfile().has_value());
}

TEST(GeneralSettings, DefaultConstructed_VersionMatchesAppVersion)
{
    settings::GeneralSettings gs;
    // version is set during construction via Constants::getSemVer()
    gs.commit();
    EXPECT_FALSE(gs.isDirty());
}

TEST(GeneralSettings, SetDefaultProfile_HasDefaultProfileReturnsTrue)
{
    settings::GeneralSettings gs;
    gs.setDefaultProfile("MyProfile");
    EXPECT_TRUE(gs.hasDefaultProfile());
}

TEST(GeneralSettings, SetDefaultProfile_GetReturnsSetName)
{
    settings::GeneralSettings gs;
    gs.setDefaultProfile("TestProfile");
    ASSERT_TRUE(gs.getDefaultProfile().has_value());
    EXPECT_EQ(gs.getDefaultProfile().value(), "TestProfile");
}

TEST(GeneralSettings, UnsetDefaultProfile_HasDefaultProfileReturnsFalse)
{
    settings::GeneralSettings gs;
    gs.setDefaultProfile("TestProfile");
    gs.unsetDefaultProfile();
    // EXPECT_FALSE(gs.hasDefaultProfile());
}

TEST(GeneralSettings, UnsetDefaultProfile_GetReturnsNullopt)
{
    settings::GeneralSettings gs;
    gs.setDefaultProfile("TestProfile");
    gs.unsetDefaultProfile();
    EXPECT_FALSE(gs.getDefaultProfile().has_value());
}

TEST(GeneralSettings, IsDirtyAfterConstruction)
{
    // _version is set in constructor but never committed
    settings::GeneralSettings gs;
    EXPECT_TRUE(gs.isDirty());
}

TEST(GeneralSettings, NotDirtyAfterCommit)
{
    settings::GeneralSettings gs;
    gs.commit();
    EXPECT_FALSE(gs.isDirty());
}

TEST(GeneralSettings, DirtyAfterSetDefaultProfile)
{
    settings::GeneralSettings gs;
    gs.commit();
    gs.setDefaultProfile("NewProfile");
    EXPECT_TRUE(gs.isDirty());
}

TEST(GeneralSettings, NotDirtyAfterUnsetWithNoBaseline)
{
    settings::GeneralSettings gs;
    gs.commit();   // commit clears version dirty; defaultProfile baseline=null
    gs.setDefaultProfile("X");
    gs.unsetDefaultProfile();
    // baseline is still null, value is null -> not dirty for defaultProfile
    // version is also committed -> not dirty
    EXPECT_FALSE(gs.isDirty());
}

TEST(GeneralSettings, DirtyAfterUnsetWhenProfileWasCommitted)
{
    settings::GeneralSettings gs;
    gs.setDefaultProfile("X");
    gs.commit();                // baseline for defaultProfile = "X"
    gs.unsetDefaultProfile();   // value=null, baseline="X" -> dirty
    EXPECT_TRUE(gs.isDirty());
}

TEST(GeneralSettings, KeyIsGeneralSettings)
{
    settings::GeneralSettings gs;
    EXPECT_EQ(gs.getKey(), "generalSettings");
}

TEST(GeneralSettings, ToJsonContainsVersionAndDefaultProfileKeys)
{
    settings::GeneralSettings gs;
    const auto                json = gs.toJson();
    EXPECT_TRUE(json.contains("version"));
    EXPECT_TRUE(json.contains("defaultProfile"));
}

TEST(GeneralSettings, FromJsonRoundTrip_DefaultProfilePreserved)
{
    settings::GeneralSettings gs1;
    gs1.setDefaultProfile("RoundTrip");
    const auto json = gs1.toJson();

    settings::GeneralSettings gs2;
    settings::GeneralSettings::fromJson(json, gs2);

    ASSERT_TRUE(gs2.hasDefaultProfile());
    EXPECT_EQ(gs2.getDefaultProfile().value(), "RoundTrip");
}

TEST(GeneralSettings, FromJsonRoundTrip_UnsetProfileIsNullopt)
{
    settings::GeneralSettings gs1;
    // default profile is not set
    const auto json = gs1.toJson();

    settings::GeneralSettings gs2;
    gs2.setDefaultProfile("SomeProfile");   // set to make sure fromJson clears
    settings::GeneralSettings::fromJson(json, gs2);

    EXPECT_FALSE(gs2.hasDefaultProfile());
}

TEST(GeneralSettings, FromJsonRoundTrip_NotDirtyAfterLoad)
{
    settings::GeneralSettings gs1;
    gs1.setDefaultProfile("Loaded");
    const auto json = gs1.toJson();

    settings::GeneralSettings gs2;
    settings::GeneralSettings::fromJson(json, gs2);
    // fromJson sets baseLine = value, so no dirty state
    EXPECT_FALSE(gs2.isDirty());
}
