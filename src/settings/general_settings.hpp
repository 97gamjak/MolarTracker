#ifndef __SETTINGS__GENERAL_SETTINGS__
#define __SETTINGS__GENERAL_SETTINGS__

#include <optional>

#include "param_container.hpp"
#include "param_container_mixin.hpp"
#include "version_param.hpp"

namespace settings
{

    class GeneralSettingsSchema
    {
       public:
        static constexpr const char* GENERAL_SETTINGS_KEY = "generalSettings";
        static constexpr const char* GENERAL_SETTINGS_TITLE =
            "General Settings";
        static constexpr const char* GENERAL_SETTINGS_DESCRIPTION =
            "Settings related to the general behavior of the application.";

        static constexpr const char* VERSION_KEY   = "version";
        static constexpr const char* VERSION_TITLE = "Version";
        static constexpr const char* VERSION_DESCRIPTION =
            "The current version of the application.";
    };

    class GeneralSettings : public ParamContainerMixin<GeneralSettings>
    {
       private:
        using Schema = GeneralSettingsSchema;
        ParamContainer _core;

        VersionParam _version{Schema::VERSION_KEY, Schema::VERSION_TITLE};

        std::optional<utils::SemVer> _currentVersion;
        std::optional<utils::SemVer> _savedVersion;

       public:
        GeneralSettings();
    };

}   // namespace settings

#endif   // __SETTINGS__GENERAL_SETTINGS__
