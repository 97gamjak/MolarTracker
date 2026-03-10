#ifndef __SETTINGS__PROFILE_UI_SETTINGS_HPP__
#define __SETTINGS__PROFILE_UI_SETTINGS_HPP__

#include <string>
#include <utility>

#include "params/params.hpp"

namespace settings
{
    /**
     * @brief Schema for the JSON representation of the profile UI settings
     *
     */
    class ProfileUISettingsSchema
    {
       public:
        /// key for the profile UI settings in JSON
        static constexpr const char* KEY = "profile_ui_settings";
        /// title for the profile UI settings
        static constexpr const char* TITLE = "Profile UI Settings";
        /// description for the profile UI settings
        static constexpr const char* DESCRIPTION =
            "Settings related to the profile UI, such as the profile list "
            "dialog, profile details dialog, etc.";

        /***************
         * Dialog Size *
         ***************/

        /// key for the profile details dialog size setting in JSON
        static constexpr const char* DIALOG_SIZE_KEY = "dialogSize";
        /// title for the profile details dialog size setting
        static constexpr const char* DIALOG_SIZE_TITLE = "Dialog Size";
        /// description for the profile details dialog size setting
        static constexpr const char* DIALOG_SIZE_DESC =
            "The size of the profile details dialog window in pixels, "
            "represented as a tuple of (width, height). For example, (800, "
            "600) "
            "would represent a dialog that is 800 pixels wide and 600 pixels "
            "tall.";
        /// default value for the profile details dialog size setting
        static constexpr const std::pair<int, int> DIALOG_SIZE_DEFAULT = {
            800,
            600
        };
    };

    /**
     * @brief Settings related to the profile UI, this includes settings for the
     * profile list dialog, profile details dialog, etc.
     *
     */
    class ProfileUISettings : public ParamContainerMixin<ProfileUISettings>
    {
       private:
        /// Schema for the JSON representation of the settings
        using Schema = ProfileUISettingsSchema;

        /// friend declaration for the ParamContainerMixin to access the private
        /// members of ProfileUISettings, this is necessary for the
        /// ParamContainerMixin to be able to access the _core member of
        /// ProfileUISettings to implement the functionality for the settings
        friend ParamContainerMixin<ProfileUISettings>;

       private:
        /// Core settings for the profile UI settings, this is used by the
        /// ParamContainerMixin to implement the functionality for the settings
        ParamContainer _core{Schema::KEY, Schema::TITLE, Schema::DESCRIPTION};

        /// The size of the profile details dialog window in pixels, represented
        /// as a tuple of (width, height)
        Numeric2DParam<int> _dialogSize{
            Schema::DIALOG_SIZE_KEY,
            Schema::DIALOG_SIZE_TITLE,
            Schema::DIALOG_SIZE_DESC
        };

       public:
        ProfileUISettings();

        [[nodiscard]] std::pair<int, int> getDialogSize() const;

       private:
        template <typename Func>
        void _forEachParam(Func&& func) const;
        template <typename Func>
        void _forEachParam(Func&& func);
    };

}   // namespace settings

#ifndef __SETTINGS__PROFILE_UI_SETTINGS_TPP__
#include "profile_ui_settings.tpp"   // IWYU pragma: keep
#endif

#endif   // __SETTINGS__PROFILE_UI_SETTINGS_HPP__