#ifndef __SETTINGS__UI_SETTINGS_HPP__
#define __SETTINGS__UI_SETTINGS_HPP__

#include "debug_slots_settings.hpp"
#include "log_viewer_settings.hpp"
#include "params/params.hpp"
#include "profile_ui_settings.hpp"

namespace settings
{

    /**
     * @brief Schema for UI settings
     *
     * This struct defines the keys, titles, and descriptions for the UI
     * settings. It serves as a centralized reference for all UI related
     * settings.
     */
    class UISettingsSchema
    {
       public:
        /// The key for the UI settings container
        static constexpr const char* KEY = "uiSettings";

        /// The title for the UI settings container
        static constexpr const char* TITLE = "UI Settings";

        /// The description for the UI settings container
        static constexpr const char* DESCRIPTION =
            "Settings related to the user interface.";
    };

    /**
     * @brief UI-related settings management
     */
    class UISettings : public ParamContainerMixin<UISettings>
    {
       private:
        /// friend declaration to allow ParamContainerMixin to access private
        /// members of UISettings, this is necessary because ParamContainerMixin
        /// needs to access the _core member of UISettings to implement the
        /// functionality for UI settings
        friend ParamContainerMixin<UISettings>;
        /// The core container for the UI settings parameters
        ParamContainer _core{
            UISettingsSchema::KEY,
            UISettingsSchema::TITLE,
            UISettingsSchema::DESCRIPTION
        };

        /// The UI settings parameters
        LogViewerSettings _logViewerSettings;

        /// The debug slots settings
        DebugSlotsSettings _debugSlotsSettings;

        /// The profile UI settings
        ProfileUISettings _profileUISettings;

       public:
        UISettings() = default;

        [[nodiscard]] LogViewerSettings&       getLogViewerSettings();
        [[nodiscard]] const LogViewerSettings& getLogViewerSettings() const;

        [[nodiscard]] DebugSlotsSettings&       getDebugSlotsSettings();
        [[nodiscard]] const DebugSlotsSettings& getDebugSlotsSettings() const;

        [[nodiscard]] ProfileUISettings&       getProfileUISettings();
        [[nodiscard]] const ProfileUISettings& getProfileUISettings() const;

       private:
        template <typename Func>
        void _forEachParam(Func&& func) const;
        template <typename Func>
        void _forEachParam(Func&& func);
    };

}   // namespace settings

#ifndef __SETTINGS__UI_SETTINGS_TPP__
#include "ui_settings.tpp"
#endif   // __SETTINGS__UI_SETTINGS_TPP__

#endif   // __SETTINGS__UI_SETTINGS_HPP__