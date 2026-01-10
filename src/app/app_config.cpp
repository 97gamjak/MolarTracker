#include "app/app_config.hpp"

#include <fstream>

#include "config/json.hpp"

namespace app
{

    AppConfig::AppConfig() { _from_json(); }

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
        jsonData["configPath"]   = _configPath;
        jsonData["databasePath"] = _databasePath;

        jsonData["defaultProfileName"] = _defaultProfileName;

        std::ofstream file{_configPath};
        if (file.is_open())
        {
            file << jsonData.dump(4);
            file.close();
        }
    }

    void AppConfig::_from_json()
    {
        std::ifstream file{_configPath};
        if (file.is_open())
        {
            nlohmann::json jsonData;
            file >> jsonData;

            _configPath   = jsonData.value("configPath", _configPath);
            _databasePath = jsonData.value("databasePath", _databasePath);
            _defaultProfileName =
                jsonData.value("defaultProfileName", _defaultProfileName);

            file.close();
        }
    }

}   // namespace app