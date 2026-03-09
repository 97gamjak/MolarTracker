#include "config/constants.hpp"

#include "exceptions/file_exceptions.hpp"
#include "utils/paths.hpp"

/**
 * @brief Get the singleton instance of Constants
 *
 * @return Constants&
 */
Constants& Constants::getInstance()
{
    static Constants instance;
    return instance;
}

/**
 * @brief Construct a new Constants:: Constants object
 *
 */
Constants::Constants()
{
    const auto& configDir = utils::configDir(Constants::getDirPrefix());

    if (configDir)
        _configPath = std::filesystem::absolute(configDir.value());
    else
    {
        // TODO(97gamjak): Add reason to exception
        // https://97gamjak.atlassian.net/browse/MOLTRACK-91
        throw DirException("Failed to resolve config directory");
    }

    const auto& dataDir = utils::dataDir(Constants::getDirPrefix());

    if (dataDir)
        _dataPath = std::filesystem::absolute(dataDir.value());
    else
    {
        // TODO(97gamjak): Add reason to exception
        // https://97gamjak.atlassian.net/browse/MOLTRACK-91
        throw DirException("Failed to resolve data directory");
    }
}

/**
 * @brief Get the configuration directory path
 *
 * @return std::filesystem::path
 */
std::filesystem::path Constants::getConfigPath() const { return _configPath; }

/**
 * @brief Get the data directory path
 *
 * @return std::filesystem::path
 */
std::filesystem::path Constants::getDataPath() const { return _dataPath; }

/**
 * @brief Get the database file path
 *
 * @return std::filesystem::path
 */
std::filesystem::path Constants::getDatabasePath() const
{
    return _dataPath / ConstantsSchema::_databaseFile;
}

/**
 * @brief Get the application name
 *
 * @return const std::string
 */
std::string Constants::getAppName() { return ConstantsSchema::_appName; }

/**
 * @brief Get the application display name, this is the name that is shown in
 * the title bar of the application, it includes the application name and
 * version
 *
 * @return const std::string
 */
std::string Constants::getAppDisplayName()
{
    return getAppName() + " " + getVersion();
}

/**
 * @brief Get the directory prefix used for config and data directories
 *
 * @return const std::string
 */
std::string Constants::getDirPrefix() { return ConstantsSchema::_dirPrefix; }

/**
 * @brief Get the desktop application name
 *
 * @return const std::string
 */
std::string Constants::getDesktopAppName()
{
    return ConstantsSchema::_desktopAppName;
}

/**
 * @brief Get the GitHub repository URL
 *
 * @return const std::string
 */
std::string Constants::getGithubRepoUrl()
{
    return ConstantsSchema::_githubRepoUrl;
}

/**
 * @brief Get the GitHub issues URL
 *
 * @return const std::string
 */
std::string Constants::getGithubIssuesUrl()
{
    return std::string(ConstantsSchema::_githubRepoUrl) + "/issues";
}

/**
 * @brief Get the application version
 *
 * @return const std::string
 */
std::string Constants::getVersion() { return ConstantsSchema::_version; }

/**
 * @brief Get the Git tag corresponding to the current version
 *
 * @return const std::string
 */
std::string Constants::getGitTag() { return ConstantsSchema::_gitTag; }

/**
 * @brief Get the database busy timeout in milliseconds, this is the amount of
 * time the application will wait for the database to become available when it
 * is locked by another process before throwing an error
 *
 * @return int
 */
int Constants::getDbBusyTimeoutMs()
{
    return ConstantsSchema::_dbBusyTimeoutMs;
}

/**
 * @brief Get the multiplier to convert seconds to milliseconds, this is used
 * for settings that are stored in seconds but need to be converted to
 * milliseconds for use in the application (e.g. auto-reload interval)
 *
 * @return int
 */
int Constants::getSecondsToMs() { return ConstantsSchema::_secondsToMs; }

/**
 * @brief Get the name of the settings file
 *
 * @return const std::string
 */
std::string Constants::getSettingsFileName()
{
    return ConstantsSchema::_settingsFileName;
}

/**
 * @brief Get the global minimum dialog size, this is used as a minimum size
 * for all dialogs in the application to ensure they are not too small to be
 * usable
 *
 * @return std::pair<int, int>
 */
std::pair<int, int> Constants::getGlobalMinDialogSize()
{
    return ConstantsSchema::_globalMinDialogSize;
}

/**
 * @brief Get the main window default size, this is used as the default size
 * for the main window when it is first created
 *
 * @return std::pair<int, int>
 */
std::pair<int, int> Constants::getMainWindowSize()
{
    return ConstantsSchema::_mainWindowSize;
}

/**
 * @brief Get the core window margins, this is used as the default margins for
 * the core content of dialogs in the application to ensure consistent spacing
 * between the edges of the dialog and the content
 *
 * @return std::tuple<int, int, int, int>
 */
std::tuple<int, int, int, int> Constants::getCoreWindowMargins()
{
    return ConstantsSchema::_coreWindowMargins;
}