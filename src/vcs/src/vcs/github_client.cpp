#include "vcs/github_client.hpp"

#include <nlohmann/json.hpp>

#include "config/constants/github_constants.hpp"
#include "http/http_client.hpp"
#include "http/http_error.hpp"
#include "http/http_request.hpp"

namespace vcs
{

    /**
     * @brief Fetch the latest release version from the GitHub Releases API.
     *
     * Calls the GitHub API endpoint for the latest MolarTracker release,
     * parses the `tag_name` field from the JSON response, strips any leading
     * `v` prefix (e.g. "v0.2.3" → "0.2.3"), and constructs a SemVer from the
     * result. Returns an error if the HTTP request fails or the response
     * cannot be parsed.
     *
     * @return std::expected<common::SemVer, http::HttpError>
     */
    std::expected<common::SemVer, http::HttpError> GitHubClient::
        fetchLatestVersion()
    {
        const auto response = http::HttpClient::get(
            http::HttpRequest{
                .url = _getLatestReleaseUrl(),
                .headers =
                    {
                        {"User-Agent", _userAgent},
                        {"Accept", "application/vnd.github+json"},
                    },
            }
        );

        if (!response)
            return std::unexpected(response.error());

        try
        {
            const auto  json = nlohmann::json::parse(response->body);
            std::string tag  = json.at("tag_name").get<std::string>();

            if (!tag.empty() && tag.front() == 'v')
                tag.erase(tag.begin());

            return common::SemVer{tag};
        }
        catch (const std::exception& e)
        {
            return std::unexpected(
                http::HttpError{
                    .kind            = http::HttpErrorKind::ParseError,
                    .message         = e.what(),
                    .responseHeaders = {},
                }
            );
        }
    }

    /**
     * @brief Get the URL for fetching the latest release information from
     * GitHub
     *
     * @return std::string
     */
    std::string GitHubClient::_getLatestReleaseUrl()
    {
        return GithubConstants::getGithubApiUrl() + "/releases/latest";
    }

}   // namespace vcs
