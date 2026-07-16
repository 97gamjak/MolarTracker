#include "settings/settings.hpp"

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>

#include "config/constants/constants.hpp"
#include "config/signal_tags.hpp"
#include "connections/connection.hpp"
#include "settings/params/param_container.hpp"
#include "settings/params/params.hpp"
#include "utils/version_json.hpp"   // IWYU pragma: keep -- needed for version param serialization

namespace settings
{
    class GeneralSettings;
    class LoggingSettings;
    class UISettings;
}   // namespace settings

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
          _settingsPath{absolute(configDir / Constants::getSettingsFileName())}
    {
        _fromJson();
    }

    /**
     * @brief Save settings to the JSON file
     *
     */
    void Settings::save()
    {
        _toJson();
        commit();
        notifySaved();
    }

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
     * @brief Get the BackupSettings object
     *
     * @return BackupSettings&
     */
    BackupSettings& Settings::getBackupSettings() { return _backupSettings; }

    /**
     * @brief Get the BackupSettings object (const version)
     *
     * @return const BackupSettings&
     */
    const BackupSettings& Settings::getBackupSettings() const
    {
        return _backupSettings;
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
            nlohmann::json jsonData = toJson();

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

            fromJson(jsonData, *this);

            file.close();
        }
    }

    /**
     * @brief subscribe to settings saved event, this allows subscribers to be
     * notified when settings are saved, the provided callback function will be
     * called whenever the settings are saved, the user pointer can be used to
     * pass additional data to the callback function, the returned Connection
     * object can be used to unsubscribe from changes
     *
     * @param func
     * @param user
     * @return Connection
     */
    Connection Settings::subscribeToSaved(OnSaved::func func, void* user)
    {
        return _onSaved.on<OnSaved>(std::move(func), user);
    }

    /**
     * @brief Notify subscribers that settings have been saved, this should be
     * called after saving settings to ensure that subscribers are notified of
     * the change
     *
     */
    void Settings::notifySaved() { _onSaved.notify<OnSaved>(); }

    /**
     * @brief check if any settings parameter is dirty, this checks the dirty
     * state of all parameters in the settings, if any parameter is dirty, this
     * function will return true, otherwise it will return false.
     *
     * @return true
     * @return false
     */
    bool Settings::isDirty() const
    {
        bool isDirty = false;

        forEachParam([&isDirty](const auto& param)
                     { isDirty |= param.isDirty(); });

        return isDirty;
    }

    /**
     * @brief Apply settings changes from JSON data, this will update the
     * settings parameters based on the provided JSON data, the exact behavior
     * of this function will depend on the structure of the JSON data and how it
     * maps to the settings parameters, but it will generally involve parsing
     * the JSON data and updating the corresponding parameters in the settings
     *
     * @param jsonData The JSON data containing the new settings values, this
     * should be a JSON object with keys corresponding to the parameter keys and
     * values corresponding to the new parameter values
     * @param settings The Settings object to apply the changes to, this is the
     * instance of the Settings class that will be updated with the new values
     */
    void Settings::fromJson(const nlohmann::json& jsonData, Settings& settings)
    {
        ParamContainerMixin<Settings>::fromJson(jsonData, settings);
    }

    /**
     * @brief Serialize settings to JSON, this will create a JSON object
     * representing the current state of the settings, the exact structure of
     * the JSON will depend on how the settings parameters serialize themselves,
     * but it will generally be a JSON object with keys corresponding to the
     * parameter keys and values corresponding to the parameter values
     *
     * @return nlohmann::json A JSON object representing the current state of
     * the settings, this can be saved to a file or used for other purposes as
     * needed
     */
    nlohmann::json Settings::toJson() const
    {
        return ParamContainerMixin<Settings>::toJson();
    }

}   // namespace settings