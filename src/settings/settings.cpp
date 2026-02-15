#include "settings.hpp"

#include <filesystem>
#include <fstream>
#include <tuple>

#include "params/params.hpp"

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
          _settingsPath{absolute(configDir / _settingsFileName)}
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
     * @brief Get the GeneralSettings object
     *
     * @return GeneralSettings&
     */
    GeneralSettings& Settings::getGeneralSettings() { return _generalSettings; }

    /**
     * @brief Get the GeneralSettings object (const version)
     *
     * @return const GeneralSettings&
     */
    const GeneralSettings& Settings::getGeneralSettings() const
    {
        return _generalSettings;
    }

    /**
     * @brief Get the UISettings object
     *
     * @return UISettings&
     */
    UISettings& Settings::getUISettings() { return _uiSettings; }

    /**
     * @brief Get the UISettings object (const version)
     *
     * @return const UISettings&
     */
    const UISettings& Settings::getUISettings() const { return _uiSettings; }

    /**
     * @brief Serialize settings to JSON and save to file
     *
     */
    void Settings::_toJson() const
    {
        std::ofstream file{_settingsPath.string()};
        if (file.is_open())
        {
            nlohmann::json jsonData = paramsToJson(_getParams());
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

            paramsFromJson(_getParams(), jsonData);

            file.close();
        }
    }

}   // namespace settings