#include "settings.hpp"

#include <filesystem>
#include <fstream>

#include "config/json.hpp"   // IWYU pragma: keep

namespace settings
{
    using std::filesystem::absolute;
    using std::filesystem::path;

    /**
     * @brief Construct a new Settings:: Settings object
     *
     * @param configDir
     */
    Settings::Settings(const path& configDir)
        : _settingsPath{absolute(configDir / _settingsFileName)}
    {
        _fromJson();
    }

    /**
     * @brief Save settings to the JSON file
     *
     */
    void Settings::save() const { _toJson(); }

    /**
     * @brief Check if a default profile is set
     *
     * @return true
     * @return false
     */
    bool Settings::hasDefaultProfile() const
    {
        return _defaultProfileName.has_value();
    }

    /**
     * @brief Get the default profile name
     *
     * @return std::optional<std::string>
     */
    std::optional<std::string> Settings::getDefaultProfileName() const
    {
        return _defaultProfileName;
    }

    /**
     * @brief Unset the default profile name
     *
     */
    void Settings::unsetDefaultProfileName() { _defaultProfileName.reset(); }

    /**
     * @brief Set the default profile name
     *
     * @param name
     */
    void Settings::setDefaultProfileName(const std::optional<std::string>& name)
    {
        _defaultProfileName = name;
    }

    /**
     * @brief Set the default profile name
     *
     * @param name
     */
    void Settings::setDefaultProfileName(const std::string& name)
    {
        _defaultProfileName = name;
    }

    /**
     * @brief Serialize settings to JSON and save to file
     *
     */
    void Settings::_toJson() const
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

    /**
     * @brief Load settings from JSON file
     *
     */
    void Settings::_fromJson()
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