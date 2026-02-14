#include "settings.hpp"

#include <filesystem>
#include <fstream>
#include <tuple>

#include "config/constants.hpp"
// #include "config/json.hpp"   // IWYU pragma: keep
#include "params/params.hpp"
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
        : _core{Schema::SETTINGS_KEY, Schema::SETTINGS_TITLE, Schema::SETTINGS_DESC},
          _settingsPath{absolute(configDir / _settingsFileName)},
          _version{utils::SemVer(Constants::getVersion())}
    {
        _fromJson();
    }

    /**
     * @brief Get the parameters of Settings as a tuple (non-const version)
     *
     * @return auto
     */
    auto Settings::_getParams() &
    {
        return std::tie(_uiSettings, _generalSettings);
    }

    /**
     * @brief Get the parameters of Settings as a tuple (const version)
     *
     * @return auto
     */
    auto Settings::_getParams() const&
    {
        return std::tie(_uiSettings, _generalSettings);
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
        nlohmann::json jsonData = paramsToJson(_getParams());

        _toJsonProfileName(jsonData);

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

            paramsFromJson(_getParams(), jsonData);

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

}   // namespace settings