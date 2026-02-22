#ifndef __SETTINGS__UI_SETTINGS_HPP__
#define __SETTINGS__UI_SETTINGS_HPP__

#include "log_viewer_settings.hpp"
#include "params/params.hpp"

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
        static constexpr const char* UI_SETTINGS_KEY = "uiSettings";

        /// The title for the UI settings container
        static constexpr const char* UI_SETTINGS_TITLE = "UI Settings";

        /// The description for the UI settings container
        static constexpr const char* UI_SETTINGS_DESC =
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
        ParamContainer _core;

        /// The UI settings parameters
        LogViewerSettings _logViewerSettings;

       public:
        UISettings();

        [[nodiscard]] LogViewerSettings&       getLogViewerSettings();
        [[nodiscard]] const LogViewerSettings& getLogViewerSettings() const;

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