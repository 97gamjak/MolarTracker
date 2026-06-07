#ifndef __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
#define __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__

#include <expected>

#include "http/http_error.hpp"
#include "utils/version.hpp"

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
        static std::expected<utils::SemVer, http::HttpError> fetchLatestVersion(
        );

       private:
        [[nodiscard]]
        static std::string _getLatestReleaseUrl();
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
