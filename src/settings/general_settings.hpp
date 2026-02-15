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
        // General settings keys, titles, and descriptions
        static constexpr const char* GENERAL_SETTINGS_KEY = "generalSettings";
        static constexpr const char* GENERAL_SETTINGS_TITLE =
            "General Settings";
        static constexpr const char* GENERAL_SETTINGS_DESCRIPTION =
            "Settings related to the general behavior of the application.";

        // Version setting keys, titles, and descriptions
        static constexpr const char* VERSION_KEY   = "version";
        static constexpr const char* VERSION_TITLE = "Version";
        static constexpr const char* VERSION_DESCRIPTION =
            "The current version of the application.";

        // Default profile setting keys, titles, and descriptions
        static constexpr const char* DEFAULT_PROFILE_KEY   = "defaultProfile";
        static constexpr const char* DEFAULT_PROFILE_TITLE = "Default Profile";
        static constexpr const char* DEFAULT_PROFILE_DESCRIPTION =
            "The default profile to use when launching the application.";
    };

    /**
     * @brief General settings management
     */
    class GeneralSettings : public ParamContainerMixin<GeneralSettings>
    {
       private:
        using Schema = GeneralSettingsSchema;
        ParamContainer _core;

        VersionParam _version{
            Schema::VERSION_KEY,
            Schema::VERSION_TITLE,
            Schema::VERSION_DESCRIPTION
        };
        StringParam _defaultProfile{
            Schema::DEFAULT_PROFILE_KEY,
            Schema::DEFAULT_PROFILE_TITLE,
            Schema::DEFAULT_PROFILE_DESCRIPTION
        };

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

        void setDefaultProfile(const std::string& profileName);
        void unsetDefaultProfile();
        [[nodiscard]] std::optional<std::string> getDefaultProfile() const;
        [[nodiscard]] bool                       hasDefaultProfile() const;

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

}   // namespace settings

#endif   // __SETTINGS__GENERAL_SETTINGS__
