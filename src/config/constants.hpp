#ifndef __CONFIG__CONSTANTS_HPP__
#define __CONFIG__CONSTANTS_HPP__

#include <filesystem>

/**
 * @brief Class containing compile-time constants for the application, these
 * are typically set during build time and are used throughout the application
 * for various purposes such as naming, paths, and URLs. This class is used by
 * the Constants singleton class to provide access to these constants throughout
 * the application.
 *
 */
class ConstantsSchema
{
    friend class Constants;   // Allow Constants to access private members of
                              // ConstantsSchema

   private:
    /// The name of the database file
    static constexpr const char* _databaseFile = "molar_tracker.db";
    /// The application name
    static constexpr const char* _appName = "Molar Tracker";
    /// The directory prefix used for config and data directories
    static constexpr const char* _dirPrefix = "molartracker";
    /// The desktop application name
    static constexpr const char* _desktopAppName =
        MOLARTRACKER_DESKTOP_APP_NAME;

    /// The GitHub repository URL
    static constexpr const char* _githubRepoUrl =
        "https://github.com/97gamjak/molartracker";

    /// The version of the application, this is set during build time using
    /// CMake.
    static constexpr const char* _version = MOLARTRACKER_VERSION_FULL;
    /// The Git tag of the application, this is set during build time using
    /// CMake.
    static constexpr const char* _gitTag = MOLARTRACKER_GIT_TAG;
};

/**
 * @brief Application-wide constants and paths, this is a singleton class
 *
 */
class Constants
{
   private:
    /// The configuration for the application, this is set during initialization
    std::filesystem::path _configPath;
    /// The data path for the application, this is set during initialization
    std::filesystem::path _dataPath;

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