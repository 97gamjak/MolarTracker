#include <gtest/gtest.h>

#include "vcs/asset_selector.hpp"

namespace vcs
{
    namespace
    {
#if defined(_WIN32)
        constexpr const char* kMatchingAssetName =
            "MolarTracker-windows-0.3.0.zip";
        constexpr const char* kOtherPlatformAssetName =
            "MolarTracker-linux-0.3.0.tar.gz";
#else
        constexpr const char* kMatchingAssetName =
            "MolarTracker-linux-0.3.0.tar.gz";
        constexpr const char* kOtherPlatformAssetName =
            "MolarTracker-windows-0.3.0.zip";
#endif
    }   // namespace

    TEST(AssetSelector, SelectsAssetMatchingCurrentPlatform)
    {
        const std::vector<ReleaseAsset> assets{
            {.name = kOtherPlatformAssetName, .downloadUrl = "other-url"},
            {.name = kMatchingAssetName, .downloadUrl = "matching-url"},
        };

        const auto selected = selectAssetForCurrentPlatform(assets);

        ASSERT_TRUE(selected.has_value());
        EXPECT_EQ(kMatchingAssetName, selected->name);
        EXPECT_EQ("matching-url", selected->downloadUrl);
    }

    TEST(AssetSelector, ReturnsNulloptWhenNoAssetMatches)
    {
        const std::vector<ReleaseAsset> assets{
            {.name = kOtherPlatformAssetName, .downloadUrl = "other-url"},
            {.name = "compile_commands-0.3.0.json", .downloadUrl = "json-url"},
        };

        EXPECT_FALSE(selectAssetForCurrentPlatform(assets).has_value());
    }

    TEST(AssetSelector, ReturnsNulloptForEmptyAssetList)
    {
        EXPECT_FALSE(selectAssetForCurrentPlatform({}).has_value());
    }

    TEST(AssetSelector, IgnoresAssetsWithMatchingPrefixButWrongExtension)
    {
        // Ends with ".sha256", not ".tar.gz"/".zip" — must not match on
        // either platform.
        const std::vector<ReleaseAsset> assets{
            {.name        = "MolarTracker-linux-0.3.0.tar.gz.sha256",
             .downloadUrl = "checksum-url"},
        };

        EXPECT_FALSE(selectAssetForCurrentPlatform(assets).has_value());
    }

}   // namespace vcs
