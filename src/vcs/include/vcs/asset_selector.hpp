#ifndef __VCS__INCLUDE__VCS__ASSET_SELECTOR_HPP__
#define __VCS__INCLUDE__VCS__ASSET_SELECTOR_HPP__

#include <optional>
#include <vector>

#include "vcs/release_asset.hpp"

namespace vcs
{

    /**
     * @brief Select the release asset matching the current platform.
     *
     * Matches "MolarTracker-linux-*.tar.gz" on Linux and
     * "MolarTracker-windows-*.zip" on Windows. Returns std::nullopt if no
     * asset in @p assets matches the current platform's naming pattern.
     *
     * @param assets The release assets to search
     * @return std::optional<ReleaseAsset>
     */
    [[nodiscard]]
    std::optional<ReleaseAsset> selectAssetForCurrentPlatform(
        const std::vector<ReleaseAsset>& assets
    );

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__ASSET_SELECTOR_HPP__
