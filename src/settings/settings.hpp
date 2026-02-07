#ifndef __SETTINGS__SETTINGS_HPP__
#define __SETTINGS__SETTINGS_HPP__

#include <filesystem>
#include <optional>
#include <string>

#include "utils/version.hpp"   // IWYU pragma: keep

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
        // clang-format on

        std::filesystem::path      _settingsPath;
        std::optional<std::string> _defaultProfileName = std::nullopt;

        utils::SemVer _version;
        utils::SemVer _oldVersion{utils::SemVer::getInvalidVersion()};

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
        void _toJson() const;
        void _fromJson();
    };

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_HPP__