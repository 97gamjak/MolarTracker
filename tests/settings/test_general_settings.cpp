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

TEST(GeneralSettings, DefaultConstructedHasNoDefaultProfile)
{
    settings::GeneralSettings settings;
    EXPECT_FALSE(settings.hasDefaultProfile());
}

TEST(GeneralSettings, DefaultConstructedGetDefaultProfileReturnsNullopt)
{
    settings::GeneralSettings settings;
    EXPECT_FALSE(settings.getDefaultProfile().has_value());
}

TEST(GeneralSettings, DefaultConstructedVersionMatchesAppVersion)
{
    settings::GeneralSettings settings;
    // version is set during construction via Constants::getSemVer()
    settings.commit();
    EXPECT_FALSE(settings.isDirty());
}

TEST(GeneralSettings, SetDefaultProfileHasDefaultProfileReturnsTrue)
{
    settings::GeneralSettings settings;
    settings.setDefaultProfile("MyProfile");
    EXPECT_TRUE(settings.hasDefaultProfile());
}

TEST(GeneralSettings, SetDefaultProfileGetReturnsSetName)
{
    settings::GeneralSettings settings;
    settings.setDefaultProfile("TestProfile");
    ASSERT_TRUE(settings.getDefaultProfile().has_value());
    EXPECT_EQ(settings.getDefaultProfile().value(), "TestProfile");
}

TEST(GeneralSettings, UnsetDefaultProfileHasDefaultProfileReturnsFalse)
{
    settings::GeneralSettings settings;
    settings.setDefaultProfile("TestProfile");
    settings.unsetDefaultProfile();
    EXPECT_FALSE(settings.hasDefaultProfile());
}

TEST(GeneralSettings, UnsetDefaultProfileGetReturnsNullopt)
{
    settings::GeneralSettings settings;
    settings.setDefaultProfile("TestProfile");
    settings.unsetDefaultProfile();
    EXPECT_FALSE(settings.getDefaultProfile().has_value());
}

TEST(GeneralSettings, IsDirtyAfterConstruction)
{
    // _version is set in constructor but never committed
    settings::GeneralSettings settings;
    EXPECT_TRUE(settings.isDirty());
}

TEST(GeneralSettings, NotDirtyAfterCommit)
{
    settings::GeneralSettings settings;
    settings.commit();
    EXPECT_FALSE(settings.isDirty());
}

TEST(GeneralSettings, DirtyAfterSetDefaultProfile)
{
    settings::GeneralSettings settings;
    settings.commit();
    settings.setDefaultProfile("NewProfile");
    EXPECT_TRUE(settings.isDirty());
}

TEST(GeneralSettings, NotDirtyAfterUnsetWithNoBaseline)
{
    settings::GeneralSettings settings;
    settings.commit(
    );   // commit clears version dirty; defaultProfile baseline=null
    settings.setDefaultProfile("X");
    settings.unsetDefaultProfile();
    // baseline is still null, value is null -> not dirty for defaultProfile
    // version is also committed -> not dirty
    EXPECT_FALSE(settings.isDirty());
}

TEST(GeneralSettings, DirtyAfterUnsetWhenProfileWasCommitted)
{
    settings::GeneralSettings settings;
    settings.setDefaultProfile("X");
    settings.commit();                // baseline for defaultProfile = "X"
    settings.unsetDefaultProfile();   // value=null, baseline="X" -> dirty
    EXPECT_TRUE(settings.isDirty());
}

TEST(GeneralSettings, KeyIsGeneralSettings)
{
    settings::GeneralSettings settings;
    EXPECT_EQ(settings.getKey(), "generalSettings");
}

TEST(GeneralSettings, ToJsonContainsVersionAndDefaultProfileKeys)
{
    settings::GeneralSettings settings;
    const auto                json = settings.toJson();
    EXPECT_TRUE(json.contains("version"));
    EXPECT_TRUE(json.contains("defaultProfile"));
}

TEST(GeneralSettings, FromJsonRoundTripDefaultProfilePreserved)
{
    settings::GeneralSettings gs1;
    gs1.setDefaultProfile("RoundTrip");
    const auto json = gs1.toJson();

    settings::GeneralSettings gs2;
    settings::GeneralSettings::fromJson(json, gs2);

    ASSERT_TRUE(gs2.hasDefaultProfile());
    EXPECT_EQ(gs2.getDefaultProfile().value(), "RoundTrip");
}

TEST(GeneralSettings, FromJsonRoundTripUnsetProfileIsNullopt)
{
    settings::GeneralSettings gs1;
    // default profile is not set
    const auto json = gs1.toJson();

    settings::GeneralSettings gs2;
    gs2.setDefaultProfile("SomeProfile");   // set to make sure fromJson clears
    settings::GeneralSettings::fromJson(json, gs2);

    EXPECT_FALSE(gs2.hasDefaultProfile());
}

TEST(GeneralSettings, FromJsonRoundTripNotDirtyAfterLoad)
{
    settings::GeneralSettings gs1;
    gs1.setDefaultProfile("Loaded");
    const auto json = gs1.toJson();

    settings::GeneralSettings gs2;
    settings::GeneralSettings::fromJson(json, gs2);
    // fromJson sets baseLine = value, so no dirty state
    EXPECT_FALSE(gs2.isDirty());
}
