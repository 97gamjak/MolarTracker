#ifndef __SETTINGS__SETTINGS_HPP__
#define __SETTINGS__SETTINGS_HPP__

#include <filesystem>
#include <optional>
#include <string>

#include "params/params.hpp"
#include "ui_settings.hpp"
#include "utils/version.hpp"

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
        ParamContainer _core;
        using Schema = SettingsSchema;

        // clang-format off
        static constexpr const char* _settingsFileName = "settings.json";
        static constexpr const char* _defaultProfileNameKey = "defaultProfileName";
        static constexpr const char* _versionKey = "version";

        static constexpr const char* _uiSettingsKey = "uiSettings";
        // clang-format on

        std::filesystem::path      _settingsPath;
        std::optional<std::string> _defaultProfileName = std::nullopt;

        utils::SemVer _version;
        utils::SemVer _oldVersion{utils::SemVer::getInvalidVersion()};

        UISettings _uiSettings;

       public:
        Settings() = delete;
        explicit Settings(const std::filesystem::path& configDir);

        void save() const;

        void unsetDefaultProfileName();
        void setDefaultProfileName(const std::optional<std::string>& name);
        void setDefaultProfileName(const std::string& name);
        [[nodiscard]] bool                       hasDefaultProfile() const;
        [[nodiscard]] std::optional<std::string> getDefaultProfileName() const;

       private:
        auto _getParams() &;
        auto _getParams() const&;

        void _toJson() const;
        void _fromJson();

        void _toJsonProfileName(nlohmann::json& jsonData) const;
        void _fromJsonProfileName(const nlohmann::json& jsonData);
        void _toJsonVersion(nlohmann::json& jsonData) const;
        void _fromJsonOldVersion(const nlohmann::json& jsonData);
    };

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_HPP__