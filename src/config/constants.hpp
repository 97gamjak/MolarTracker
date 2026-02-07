#ifndef __CONFIG__CONSTANTS_HPP__
#define __CONFIG__CONSTANTS_HPP__

#include <filesystem>

/**
 * @brief Application-wide constants and paths, this is a singleton class
 *
 */
class Constants
{
   private:
    std::filesystem::path _configPath;
    std::filesystem::path _dataPath;

    // clang-format off
    static constexpr const char* _databaseFile    = "molar_tracker.db";
    static constexpr const char* _appName         = "Molar Tracker";
    static constexpr const char* _dirPrefix       = "molartracker";
    static constexpr const char* _desktopAppName  = MOLARTRACKER_DESKTOP_APP_NAME;
    static constexpr const char* _githubRepoUrl   = "https://github.com/97gamjak/molartracker";

    static constexpr const char* _version         = MOLARTRACKER_VERSION_FULL;
    static constexpr const char* _gitTag          = MOLARTRACKER_GIT_TAG;
    // clang-format on

   public:
    static Constants& getInstance();

    [[nodiscard]] std::filesystem::path getConfigPath() const;
    [[nodiscard]] std::filesystem::path getDataPath() const;
    [[nodiscard]] std::filesystem::path getDatabasePath() const;

    [[nodiscard]] static const std::string getAppName();
    [[nodiscard]] static const std::string getDirPrefix();
    [[nodiscard]] static const std::string getDesktopAppName();
    [[nodiscard]] static const std::string getGithubRepoUrl();
    [[nodiscard]] static const std::string getGithubIssuesUrl();

    [[nodiscard]] static const std::string getVersion();
    [[nodiscard]] static const std::string getGitTag();

   private:
    Constants();
};

#endif   // __CONFIG__CONSTANTS_HPP__