#include "config/constants/github_constants.hpp"

/**
 * @brief Get the GitHub repository URL
 *
 * @return const std::string
 */
std::string GithubConstants::getGithubRepoUrl()
{
    return std::string(_githubUrl) + _repoPath;
}

/**
 * @brief Get the GitHub API URL
 *
 * @return const std::string
 */
std::string GithubConstants::getGithubApiUrl()
{
    return std::string(_githubApiUrl) + _repoPath;
}

/**
 * @brief Get the GitHub release URL
 *
 * @return const std::string
 */
std::string GithubConstants::getGithubReleaseUrl()
{
    return getGithubRepoUrl() + "/releases";
}

/**
 * @brief Get the GitHub issues URL
 *
 * @return const std::string
 */
std::string GithubConstants::getGithubIssuesUrl()
{
    return getGithubRepoUrl() + "/issues";
}

/**
 * @brief Get the error message to display when creating a GitHub issue fails
 *
 * @return const std::string
 */
std::string GithubConstants::getCreateIssueError()
{
    return "This should never happen. If you see this message, please report "
           "it to the developers under " +
           GithubConstants::getGithubIssuesUrl();
}

/**
 * @brief Get the semantic version of the application
 *
 * @return utils::SemVer
 */
utils::SemVer GithubConstants::getSemVer() { return utils::SemVer(version); }