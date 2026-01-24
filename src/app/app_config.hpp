#ifndef __APP__APP_CONFIG_HPP__
#define __APP__APP_CONFIG_HPP__

#include <filesystem>
#include <optional>
#include <string>

namespace app
{

    /**
     * @brief Application configuration management
     */
    class AppConfig
    {
       private:
        // clang-format off
        static constexpr const char* _configFileName = "config.json";
        static constexpr const char* _defaultProfileNameKey = "defaultProfileName";
        // clang-format on

        std::filesystem::path      _configPath;
        std::optional<std::string> _defaultProfileName = std::nullopt;

       public:
        AppConfig() = delete;
        explicit AppConfig(const std::filesystem::path& configDir);

        void save() const;

        [[nodiscard]] bool                       has_default_profile() const;
        [[nodiscard]] std::optional<std::string> get_default_profile_name(
        ) const;
        void set_default_profile_name(const std::string& name);

       private:
        void _to_json() const;
        void _from_json();
    };

}   // namespace app

#endif   // __APP__APP_CONFIG_HPP__