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
        std::expected<utils::SemVer, http::HttpError> fetchLatestVersion(
        ) const;

       private:
        static constexpr std::string_view k_url =
            "https://api.github.com/repos/97gamjak/MolarTracker/releases/"
            "latest";
        static constexpr std::string_view k_userAgent = "MolarTracker";
    };

}   // namespace vcs

#endif   // __VCS__INCLUDE__VCS__GITHUB_CLIENT_HPP__
