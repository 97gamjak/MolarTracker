#include "settings.hpp"

#include <filesystem>
#include <fstream>

#include "config/json.hpp"

namespace settings
{

    Settings::Settings(const std::filesystem::path& configDir)
        : _settingsPath{
              std::filesystem::absolute(configDir / _settingsFileName)
          }
    {
        _from_json();
    }

    void Settings::save() const { _to_json(); }

    bool Settings::has_default_profile() const
    {
        return _defaultProfileName.has_value();
    }

    std::optional<std::string> Settings::get_default_profile_name() const
    {
        return _defaultProfileName;
    }

    void Settings::set_default_profile_name(const std::string& name)
    {
        _defaultProfileName = name;
    }

    void Settings::_to_json() const
    {
        nlohmann::json jsonData;
        jsonData[_defaultProfileNameKey] = _defaultProfileName;

        std::ofstream file{_settingsPath.string()};
        if (file.is_open())
        {
            file << jsonData.dump(4);
            file.close();
        }
    }

    void Settings::_from_json()
    {
        std::ifstream file{_settingsPath.string()};
        if (file.is_open())
        {
            nlohmann::json jsonData;
            file >> jsonData;

            _defaultProfileName =
                jsonData.value(_defaultProfileNameKey, _defaultProfileName);

            file.close();
        }
    }

}   // namespace settings