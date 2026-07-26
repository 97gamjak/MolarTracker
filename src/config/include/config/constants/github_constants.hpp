#ifndef __CONFIG__INCLUDE__CONFIG__CONSTANTS__GITHUB_CONSTANTS_HPP__
#define __CONFIG__INCLUDE__CONFIG__CONSTANTS__GITHUB_CONSTANTS_HPP__

#include <string>

#include "common/version.hpp"

/**
 * @brief Class containing compile-time constants related to GitHub for the
 * application, these are typically set during build time and are used
 * throughout the application for various purposes such as URLs and error
 * messages related to GitHub.
 *
 */
class GithubConstants
{
   private:
    /// The GitHub URL
    static constexpr const char* _githubUrl = "https://github.com/";
    /// The GitHub API URL
    static constexpr const char* _githubApiUrl =
        "https://api.github.com/repos/";
    /// The GitHub repository path for the application
    static constexpr const char* _repoPath = "97gamjak/MolarTracker";

   public:
    /// The version of the application, this is set during build time using
    /// CMake.
    static constexpr const char* version = MOLARTRACKER_VERSION_FULL;
    /// The Git tag of the application, this is set during build time using
    /// CMake.
    static constexpr const char* gitTag = MOLARTRACKER_GIT_TAG;

    [[nodiscard]] static std::string getGithubRepoUrl();
    [[nodiscard]] static std::string getGithubApiUrl();
    [[nodiscard]] static std::string getGithubReleaseUrl();
    [[nodiscard]] static std::string getGithubIssuesUrl();
    [[nodiscard]] static std::string getCreateIssueError();

    [[nodiscard]] static common::SemVer getSemVer();
};

#endif   // __CONFIG__INCLUDE__CONFIG__CONSTANTS__GITHUB_CONSTANTS_HPP__