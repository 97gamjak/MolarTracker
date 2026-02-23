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
    const auto& configDir = utils::config_dir(Constants::getDirPrefix());

    if (configDir)
        _configPath = std::filesystem::absolute(configDir.value());
    else
    {
        // TODO(97gamjak): Add reason to exception
        // https://97gamjak.atlassian.net/browse/MOLTRACK-91
        throw DirException("Failed to resolve config directory");
    }

    const auto& dataDir = utils::data_dir(Constants::getDirPrefix());

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