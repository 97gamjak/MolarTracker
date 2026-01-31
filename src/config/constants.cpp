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
    const auto& configDir = utils::config_dir(Constants::DIR_PREFIX);

    if (configDir)
        _configPath = std::filesystem::absolute(configDir.value());
    else
        throw DirException("Failed to resolve config directory");

    const auto& dataDir = utils::data_dir(Constants::DIR_PREFIX);

    if (dataDir)
        _dataPath = std::filesystem::absolute(dataDir.value());
    else
    {
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
    return _dataPath / _databaseFile;
}