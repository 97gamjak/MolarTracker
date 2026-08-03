#ifndef __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
#define __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__

#include <vector>

#include "common/version.hpp"
#include "error/http_error.hpp"
#include "vcs/release_asset.hpp"

namespace vcs
{

    /**
     * @brief Fetches the latest release version from the GitHub Releases API.
     *
     */
    class GitHubClient
    {
       private:
        /// The User-Agent header value to use for GitHub API requests
        static constexpr const char* _userAgent = "MolarTracker";

       public:
        [[nodiscard]]
        static HttpResult<common::SemVer> fetchLatestVersion();

        [[nodiscard]]
        static HttpResult<std::vector<ReleaseAsset>> fetchLatestReleaseAssets();

       private:
        [[nodiscard]]
        static std::string _getLatestReleaseUrl();
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
