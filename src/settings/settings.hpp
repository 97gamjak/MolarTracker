#ifndef __SETTINGS__SETTINGS_HPP__
#define __SETTINGS__SETTINGS_HPP__

#include <filesystem>
#include <optional>
#include <string>

#include "nlohmann/json.hpp"
#include "ui_settings.hpp"
#include "utils/version.hpp"

namespace settings
{

    /**
     * @brief Application settings management
     */
    class Settings
    {
       private:
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

       public:   // getters and setters
        void unsetDefaultProfileName();
        void setDefaultProfileName(const std::optional<std::string>& name);
        void setDefaultProfileName(const std::string& name);
        [[nodiscard]] bool                       hasDefaultProfile() const;
        [[nodiscard]] std::optional<std::string> getDefaultProfileName() const;

        [[nodiscard]] UISettings&       getUISettings();
        [[nodiscard]] const UISettings& getUISettings() const;

       private:
        void _toJson() const;
        void _fromJson();

        void _toJsonProfileName(nlohmann::json& jsonData) const;
        void _fromJsonProfileName(const nlohmann::json& jsonData);
        void _toJsonVersion(nlohmann::json& jsonData) const;
        void _fromJsonOldVersion(const nlohmann::json& jsonData);
        void _toJsonUISettings(nlohmann::json& jsonData) const;
        void _fromJsonUISettings(const nlohmann::json& jsonData);
    };

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_HPP__