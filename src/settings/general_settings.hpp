#ifndef __SETTINGS__GENERAL_SETTINGS_HPP__
#define __SETTINGS__GENERAL_SETTINGS_HPP__

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
        /********************
         * General Settings *
         ********************/

        /// general settings key
        static constexpr const char* GENERAL_SETTINGS_KEY = "generalSettings";
        /// general settings title
        static constexpr const char* GENERAL_SETTINGS_TITLE =
            "General Settings";
        /// general settings description
        static constexpr const char* GENERAL_SETTINGS_DESCRIPTION =
            "Settings related to the general behavior of the application.";

        /***********
         * Version *
         ***********/

        /// version key
        static constexpr const char* VERSION_KEY = "version";
        /// version title
        static constexpr const char* VERSION_TITLE = "Version";
        /// version description
        static constexpr const char* VERSION_DESCRIPTION =
            "The current version of the application.";

        /****************************
         * Default Profile Settings *
         ****************************/

        /// default profile key
        static constexpr const char* DEFAULT_PROFILE_KEY = "defaultProfile";
        /// default profile title
        static constexpr const char* DEFAULT_PROFILE_TITLE = "Default Profile";
        /// default profile description
        static constexpr const char* DEFAULT_PROFILE_DESCRIPTION =
            "The default profile to use when launching the application.";
    };

    /**
     * @brief General settings management
     */
    class GeneralSettings : public ParamContainerMixin<GeneralSettings>
    {
       private:
        /// type alias for GeneralSettingsSchema
        using Schema = GeneralSettingsSchema;

        /// friend declaration to allow ParamContainerMixin to access private
        /// members of GeneralSettings, this is necessary because
        /// ParamContainerMixin needs to access the _core member of
        /// GeneralSettings to implement the functionality for general settings
        friend ParamContainerMixin<GeneralSettings>;

        /// The core container for the general settings parameters
        ParamContainer _core;

        /// The version of the application
        VersionParam _version{
            Schema::VERSION_KEY,
            Schema::VERSION_TITLE,
            Schema::VERSION_DESCRIPTION
        };

        /// The default profile to use when launching the application
        StringParam _defaultProfile{
            Schema::DEFAULT_PROFILE_KEY,
            Schema::DEFAULT_PROFILE_TITLE,
            Schema::DEFAULT_PROFILE_DESCRIPTION
        };

        /// The current version of the application
        std::optional<utils::SemVer> _currentVersion;

        /// The version of the application when the settings were last saved
        std::optional<utils::SemVer> _savedVersion;

       public:
        GeneralSettings();

        [[nodiscard]] nlohmann::json toJson() const;

        static void fromJson(
            const nlohmann::json& j,
            GeneralSettings&      settings
        );

        void               setDefaultProfile(const std::string& profileName);
        void               unsetDefaultProfile();
        [[nodiscard]] bool hasDefaultProfile() const;
        [[nodiscard]] const std::optional<std::string>& getDefaultProfile(
        ) const;

       private:
        [[nodiscard]] auto _getParams() const&;
        [[nodiscard]] auto _getParams() &;
    };

}   // namespace settings

#endif   // __SETTINGS__GENERAL_SETTINGS_HPP__
