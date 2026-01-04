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

        void                       save() const;
        bool                       has_default_profile() const;
        std::optional<std::string> get_default_profile_name() const;

       private:
        void _to_json() const;
        void _from_json();
    };

}   // namespace app

#endif   // __APP__APP_CONFIG_HPP__