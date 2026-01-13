#ifndef __APP__APP_CONFIG_HPP__
#define __APP__APP_CONFIG_HPP__

#include <optional>
#include <string>

namespace app
{

    class AppConfig
    {
        std::string                _configPath         = "config.json";
        std::string                _databasePath       = "molar_tracker.db";
        std::optional<std::string> _defaultProfileName = std::nullopt;

       public:
        AppConfig();

        void save() const;

        [[nodiscard]] bool has_default_profile() const;
        [[nodiscard]] std::optional<std::string> get_default_profile_name() const;
        void set_default_profile_name(const std::string& name);

        [[nodiscard]] std::string get_database_path() const;

       private:
        void _to_json() const;
        void _from_json();
    };

}   // namespace app

#endif   // __APP__APP_CONFIG_HPP__