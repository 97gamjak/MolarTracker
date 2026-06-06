#ifndef __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
#define __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__

#include <expected>
#include <string_view>

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
       public:
        [[nodiscard]]
        static std::expected<utils::SemVer, http::HttpError> fetchLatestVersion(
        );

       private:
        /// The GitHub API endpoint for fetching the latest release information.
        static constexpr std::string_view k_url =
            "https://api.github.com/repos/97gamjak/MolarTracker/releases/"
            "latest";
        static constexpr std::string_view k_userAgent = "MolarTracker";
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
