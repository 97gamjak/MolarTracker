#include "settings.hpp"

#include <filesystem>
#include <fstream>

#include "config/constants.hpp"
#include "config/json.hpp"   // IWYU pragma: keep
#include "utils/version.hpp"

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
        : _settingsPath{absolute(configDir / _settingsFileName)},
          _version{utils::SemVer(Constants::getVersion())}
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
     * @brief Get the UI settings
     *
     * @return UISettings&
     */
    UISettings& Settings::getUISettings() { return _uiSettings; }

    /**
     * @brief Get the UI settings (const version)
     *
     * @return const UISettings&
     */
    const UISettings& Settings::getUISettings() const { return _uiSettings; }

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

        _toJsonProfileName(jsonData);
        _toJsonVersion(jsonData);
        _toJsonUISettings(jsonData);

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

            _fromJsonProfileName(jsonData);
            _fromJsonOldVersion(jsonData);
            _fromJsonUISettings(jsonData);

            file.close();
        }
    }

    /**
     * @brief Serialize the default profile name to JSON
     *
     * @param jsonData
     */
    void Settings::_toJsonProfileName(nlohmann::json& jsonData) const
    {
        jsonData[_defaultProfileNameKey] = _defaultProfileName;
    }

    /**
     * @brief Deserialize the default profile name from JSON
     *
     * @param jsonData
     */
    void Settings::_fromJsonProfileName(const nlohmann::json& jsonData)
    {
        const auto key          = _defaultProfileNameKey;
        const auto defaultValue = _defaultProfileName;

        _defaultProfileName = jsonData.value(key, defaultValue);
    }

    /**
     * @brief Serialize the version to JSON
     *
     * @param jsonData
     */
    void Settings::_toJsonVersion(nlohmann::json& jsonData) const
    {
        jsonData[_versionKey] = _version;
    }

    /**
     * @brief Deserialize the old version from JSON
     *
     * @param jsonData
     */
    void Settings::_fromJsonOldVersion(const nlohmann::json& jsonData)
    {
        const auto key          = _versionKey;
        const auto defaultValue = _oldVersion;

        _oldVersion = jsonData.value(key, defaultValue);
    }

    /**
     * @brief Serialize UI settings to JSON
     *
     * @param jsonData
     */
    void Settings::_toJsonUISettings(nlohmann::json& jsonData) const
    {
        jsonData[_uiSettingsKey] = _uiSettings;
    }

    void Settings::_fromJsonUISettings(const nlohmann::json& jsonData)
    {
        _uiSettings = jsonData.value(_uiSettingsKey, _uiSettings);
    }

}   // namespace settings