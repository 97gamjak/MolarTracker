#ifndef __SETTINGS__SETTINGS_HPP__
#define __SETTINGS__SETTINGS_HPP__

#include <filesystem>
#include <optional>
#include <string>

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
        // clang-format on

        std::filesystem::path      _settingsPath;
        std::optional<std::string> _defaultProfileName = std::nullopt;

       public:
        Settings() = delete;
        explicit Settings(const std::filesystem::path& configDir);

        void save() const;

        [[nodiscard]] bool                       has_default_profile() const;
        [[nodiscard]] std::optional<std::string> get_default_profile_name(
        ) const;
        void set_default_profile_name(const std::optional<std::string>& name);
        void set_default_profile_name(const std::string& name);

       private:
        void _to_json() const;
        void _from_json();
    };

}   // namespace settings

#endif   // __SETTINGS__SETTINGS_HPP__