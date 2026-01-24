#include "config/constants.hpp"

#include "exceptions/file_exceptions.hpp"
#include "utils/paths.hpp"

Constants& Constants::getInstance()
{
    static Constants instance;
    return instance;
}

Constants::Constants()
{
    const auto& configDir = utils::config_dir(Constants::DIR_PREFIX);

    if (configDir)
        _configPath = std::filesystem::absolute(configDir.value());
    else
        throw DirException(
            "Failed to resolve config directory"
        );   // TODO: Add reason why

    const auto& dataDir = utils::data_dir(Constants::DIR_PREFIX);

    if (dataDir)
        _dataPath = std::filesystem::absolute(dataDir.value());
    else
    {
        throw DirException(
            "Failed to resolve data directory"
        );   // TODO: Add reason why
    }
}

std::filesystem::path Constants::getConfigPath() const { return _configPath; }

std::filesystem::path Constants::getDataPath() const { return _dataPath; }

std::filesystem::path Constants::getDatabasePath() const
{
    return _dataPath / _databaseFile;
}