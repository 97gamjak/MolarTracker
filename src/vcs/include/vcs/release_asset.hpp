#ifndef __VCS__INCLUDE__VCS__RELEASE_ASSET_HPP__
#define __VCS__INCLUDE__VCS__RELEASE_ASSET_HPP__

#include <string>

namespace vcs
{

    /**
     * @brief A single downloadable asset attached to a GitHub release.
     *
     */
    struct ReleaseAsset
    {
        /// The asset's file name (e.g. "MolarTracker-linux-0.3.0.tar.gz")
        std::string name;
        /// The direct download URL for the asset
        std::string downloadUrl;
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__RELEASE_ASSET_HPP__
