#ifndef __SETTINGS__SETTINGS_HPP__
#define __SETTINGS__SETTINGS_HPP__

#include <filesystem>

#include "general_settings.hpp"
#include "params/params.hpp"
#include "ui_settings.hpp"

namespace settings
{
    class SettingsSchema
    {
       public:
        static constexpr const char* SETTINGS_KEY   = "settings";
        static constexpr const char* SETTINGS_TITLE = "Settings";
        static constexpr const char* SETTINGS_DESC  = "Settings";
    };

    /**
     * @brief Application settings management
     */
    class Settings : public ParamContainerMixin<Settings>
    {
       private:
        using Schema = SettingsSchema;

        ParamContainer _core;

        // clang-format off
        static constexpr const char* _settingsFileName = "settings.json";
        // clang-format on

        std::filesystem::path _settingsPath;

        GeneralSettings _generalSettings;
        UISettings      _uiSettings;

       public:
        Settings() = delete;
        explicit Settings(const std::filesystem::path& configDir);

        void save() const;

       public:   // getters and setters
        [[nodiscard]] GeneralSettings&       getGeneralSettings();
        [[nodiscard]] const GeneralSettings& getGeneralSettings() const;

        [[nodiscard]] UISettings&       getUISettings();
        [[nodiscard]] const UISettings& getUISettings() const;

       private:
        auto _getParams() &;
        auto _getParams() const&;

        void _toJson() const;
        void _fromJson();
    };

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_HPP__