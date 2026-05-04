#ifndef __CONFIG__INCLUDE__CONFIG__CONSTANTS_HPP__
#define __CONFIG__INCLUDE__CONFIG__CONSTANTS_HPP__

#include <filesystem>
#include <string>
#include <tuple>
#include <utility>

#include "utils/version.hpp"

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

    /// The busy timeout for the database in milliseconds
    static constexpr int _dbBusyTimeoutMs = 5000;

    /// The name of the settings file
    static constexpr const char* _settingsFileName = "settings.json";

    // TODO(97gamjak): move this to a specialized quantity approach
    // as soon as it is implemented in mstd
    /// The multiplier to convert seconds to milliseconds
    static constexpr const int _secondsToMs = 1000;

    /// The global minimum dialog size for all dialogs in the application, this
    /// is used to ensure that all dialogs have a reasonable minimum size and
    /// are not too small to be usable
    static constexpr const std::pair<int, int> _globalMinDialogSize = {
        400,
        300
    };

    /// The margins for the core content of dialogs in the application, this is
    /// used to ensure that there is consistent spacing between the edges of the
    /// dialog and the content, this is represented as a tuple of (left, top,
    /// right, bottom) margins in pixels
    static constexpr const std::tuple<int, int, int, int> _coreWindowMargins =
        {8, 8, 8, 8};

    /// Main window default size, this is used as the default size for the main
    /// window when it is first created
    static constexpr const std::pair<int, int> _mainWindowSize = {4000, 3000};

    /// Width of the side bar
    static constexpr const int _sideBarWidth = 120;
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

    [[nodiscard]] static std::string getAppName();
    [[nodiscard]] static std::string getAppDisplayName();
    [[nodiscard]] static std::string getDirPrefix();
    [[nodiscard]] static std::string getDesktopAppName();
    [[nodiscard]] static std::string getGithubRepoUrl();
    [[nodiscard]] static std::string getGithubIssuesUrl();

    [[nodiscard]] static std::string   getVersion();
    [[nodiscard]] static std::string   getGitTag();
    [[nodiscard]] static utils::SemVer getSemVer();

    [[nodiscard]] static int getDbBusyTimeoutMs();

    // TODO(97gamjak): move this to a specialized quantity approach
    // as soon as it is implemented in mstd
    [[nodiscard]] static int getSecondsToMs();

    [[nodiscard]] static std::string getSettingsFileName();

    [[nodiscard]] static std::pair<int, int> getGlobalMinDialogSize();
    [[nodiscard]] static std::pair<int, int> getMainWindowSize();
    [[nodiscard]] static std::tuple<int, int, int, int> getCoreWindowMargins();
    [[nodiscard]] static int                            getSideBarWidth();

   private:
    Constants();
};

#endif   // __CONFIG__INCLUDE__CONFIG__CONSTANTS_HPP__