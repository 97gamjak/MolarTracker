#include "settings.hpp"

#include <filesystem>
#include <fstream>

#include "params/params.hpp"

namespace settings
{
    using std::filesystem::absolute;

    /**
     * @brief Construct a new Settings:: Settings object
     *
     * @param configDir
     */
    Settings::Settings(const std::filesystem::path& configDir)
        : _core{SettingsSchema::SETTINGS_KEY, SettingsSchema::SETTINGS_TITLE, SettingsSchema::SETTINGS_DESC},
          _settingsPath{absolute(configDir / _settingsFileName)}
    {
        _fromJson();
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
     * @brief Get the LoggingSettings object
     *
     * @return LoggingSettings&
     */
    LoggingSettings& Settings::getLoggingSettings() { return _loggingSettings; }

    /**
     * @brief Get the LoggingSettings object (const version)
     *
     * @return const LoggingSettings&
     */
    const LoggingSettings& Settings::getLoggingSettings() const
    {
        return _loggingSettings;
    }

    /**
     * @brief Serialize settings to JSON and save to file
     *
     */
    void Settings::_toJson() const
    {
        std::ofstream file{_settingsPath.string()};
        if (file.is_open())
        {
            nlohmann::json jsonData;

            auto toJsonFunc = [&](const auto& param)
            { jsonData[param.getKey()] = param.toJson(); };

            _forEachParam(toJsonFunc);

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

            auto fromJsonFunc = [&](auto& param)
            {
                const auto& key = param.getKey();
                if (jsonData.contains(key))
                {
                    std::remove_cvref_t<decltype(param)>::fromJson(
                        jsonData[key],
                        param
                    );
                }
            };

            _forEachParam(fromJsonFunc);

            file.close();
        }
    }

}   // namespace settings