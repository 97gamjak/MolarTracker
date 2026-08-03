#include "vcs/asset_selector.hpp"

#include <algorithm>

namespace vcs
{

    namespace
    {
#if defined(_WIN32)
        constexpr const char* _platformPrefix = "MolarTracker-windows-";
        constexpr const char* _platformSuffix = ".zip";
#else
        constexpr const char* _platformPrefix = "MolarTracker-linux-";
        constexpr const char* _platformSuffix = ".tar.gz";
#endif
    }   // namespace

    /**
     * @brief Select the release asset matching the current platform.
     *
     * @param assets The release assets to search
     * @return std::optional<ReleaseAsset>
     */
    std::optional<ReleaseAsset> selectAssetForCurrentPlatform(
        const std::vector<ReleaseAsset>& assets
    )
    {
        const auto it = std::ranges::find_if(
            assets,
            [](const ReleaseAsset& asset)
            {
                return asset.name.starts_with(_platformPrefix) &&
                       asset.name.ends_with(_platformSuffix);
            }
        );

        if (it == assets.end())
            return std::nullopt;

        return *it;
    }

}   // namespace vcs
