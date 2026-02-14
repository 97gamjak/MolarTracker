#ifndef __SETTINGS__GENERAL_SETTINGS__
#define __SETTINGS__GENERAL_SETTINGS__

#include <optional>

#include "params/params.hpp"

namespace settings
{

    /**
     * @brief Schema for general settings
     *
     * This struct defines the keys, titles, and descriptions for the general
     * settings. It serves as a centralized reference for all general settings.
     */
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

    /**
     * @brief General settings management
     */
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

        [[nodiscard]] ParamContainer&       core();
        [[nodiscard]] const ParamContainer& core() const;

        [[nodiscard]] nlohmann::json toJson() const;

        static void fromJson(
            const nlohmann::json& j,
            GeneralSettings&      settings
        );

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

}   // namespace settings

#endif   // __SETTINGS__GENERAL_SETTINGS__
