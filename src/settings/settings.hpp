#ifndef __SETTINGS__SETTINGS_HPP__
#define __SETTINGS__SETTINGS_HPP__

#include <filesystem>

#include "general_settings.hpp"
#include "logging_settings.hpp"
#include "params/params.hpp"
#include "ui_settings.hpp"

namespace settings
{
    /**
     * @brief Schema for the application settings. This defines the keys and
     * descriptions for the settings parameters.
     *
     */
    class SettingsSchema
    {
       public:
        /// The key for the settings container
        static constexpr const char* SETTINGS_KEY = "settings";

        /// The title for the settings container
        static constexpr const char* SETTINGS_TITLE = "Settings";

        /// The description for the settings container
        static constexpr const char* SETTINGS_DESC = "Settings";
    };

    /**
     * @brief Application settings management
     */
    class Settings : public ParamContainerMixin<Settings>
    {
       private:
        /// The core container for the settings parameters
        ParamContainer _core;

        // TODO: maybe move this somewhere else
        /// The name of the settings file
        static constexpr const char* _settingsFileName = "settings.json";

        /// The path to the settings file
        std::filesystem::path _settingsPath;

        /// The general settings parameters
        GeneralSettings _generalSettings;

        /// The UI settings parameters
        UISettings _uiSettings;

        /// The logging settings parameters
        LoggingSettings _loggingSettings;

       public:
        Settings() = delete;
        explicit Settings(const std::filesystem::path& configDir);

        void save() const;

       public:   // getters and setters
        [[nodiscard]] GeneralSettings&       getGeneralSettings();
        [[nodiscard]] const GeneralSettings& getGeneralSettings() const;

        [[nodiscard]] UISettings&       getUISettings();
        [[nodiscard]] const UISettings& getUISettings() const;

        [[nodiscard]] LoggingSettings&       getLoggingSettings();
        [[nodiscard]] const LoggingSettings& getLoggingSettings() const;

       private:
        auto _getParams() &;
        auto _getParams() const&;

        void _toJson() const;
        void _fromJson();
    };

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_HPP__