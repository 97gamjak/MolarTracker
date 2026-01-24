#include "app/app_config.hpp"

#include <filesystem>
#include <fstream>

#include "config/json.hpp"

namespace app
{

    AppConfig::AppConfig(const std::filesystem::path& configDir)
        : _configPath{std::filesystem::absolute(configDir / _configFileName)}
    {
        _from_json();
    }

    void AppConfig::save() const { _to_json(); }

    bool AppConfig::has_default_profile() const
    {
        return _defaultProfileName.has_value();
    }

    std::optional<std::string> AppConfig::get_default_profile_name() const
    {
        return _defaultProfileName;
    }

    void AppConfig::set_default_profile_name(const std::string& name)
    {
        _defaultProfileName = name;
    }

    void AppConfig::_to_json() const
    {
        nlohmann::json jsonData;
        jsonData[_defaultProfileNameKey] = _defaultProfileName;

        std::ofstream file{_configPath.string()};
        if (file.is_open())
        {
            file << jsonData.dump(4);
            file.close();
        }
    }

    void AppConfig::_from_json()
    {
        std::ifstream file{_configPath.string()};
        if (file.is_open())
        {
            nlohmann::json jsonData;
            file >> jsonData;

            _defaultProfileName =
                jsonData.value(_defaultProfileNameKey, _defaultProfileName);

            file.close();
        }
    }

}   // namespace app