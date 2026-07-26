#ifndef __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_HPP__
#define __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_HPP__

#include <string>

#include "common/shortcut.hpp"
#include "settings/params/params.hpp"

namespace settings
{

    /**
     * @brief Schema for general settings
     *
     * This struct defines the keys, titles, and descriptions for the general
     * settings. It serves as a centralized reference for all general settings.
     */
    class ShortcutSettingsSchema
    {
       public:
        /********************
         * Shortcut Settings *
         ********************/

        /// shortcut settings key
        static constexpr const char* SHORTCUT_SETTINGS_KEY = "shortcutSettings";
        /// shortcut settings title
        static constexpr const char* SHORTCUT_SETTINGS_TITLE =
            "Shortcut Settings";
        /// shortcut settings description
        static constexpr const char* SHORTCUT_SETTINGS_DESCRIPTION =
            "Settings related to the keyboard shortcuts of the application.";

        /*****************
         * Shortcut list *
         *****************/

        /// shortcut list key
        static constexpr const char* SHORTCUT_LIST_KEY = "shortcutList";
        /// shortcut list title
        static constexpr const char* SHORTCUT_LIST_TITLE = "Shortcut List";
        /// shortcut list description
        static constexpr const char* SHORTCUT_LIST_DESCRIPTION =
            "The list of keyboard shortcuts for the application.";

        /*******************
         * Single Shortcut *
         *******************/

        static constexpr const char* SAVE_SHORTCUT_KEY   = "saveShortcut";
        static constexpr const char* SAVE_SHORTCUT_TITLE = "Save Shortcut";
        static constexpr const char* SAVE_SHORTCUT_DESCRIPTION =
            "The keyboard shortcut for saving the current state of the "
            "application.";
        static constexpr std::tuple<ShortcutModifier, char, std::string>
            SAVE_SHORTCUT_DEFAULT = {ShortcutModifier::Control, 'S', "Save"};
    };

    /**
     * @brief Shortcut settings management
     */
    class ShortcutSettings : public ParamContainerMixin<ShortcutSettings>
    {
       private:
        /// type alias for ShortcutSettingsSchema
        using Schema = ShortcutSettingsSchema;

        /// friend declaration to allow ParamContainerMixin to access private
        /// members of ShortcutSettings, this is necessary because
        /// ParamContainerMixin needs to access the _core member of
        /// ShortcutSettings to implement the functionality for shortcut
        /// settings
        friend ParamContainerMixin<ShortcutSettings>;

        /// The core container for the general settings parameters
        ParamContainer _core;

        /// The version of the application
        VecParam<Shortcut> _shortcuts{
            Schema::SHORTCUT_LIST_KEY,
            Schema::SHORTCUT_LIST_TITLE,
            Schema::SHORTCUT_LIST_DESCRIPTION
        };

       public:
        ShortcutSettings();

        template <typename Func>
        void forEachParam(Func&& func) const;
        template <typename Func>
        void forEachParam(Func&& func);
    };

}   // namespace settings

#ifndef __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_TPP__
#include "shortcut_settings.tpp"
#endif

#endif   // __SETTINGS__INCLUDE__SETTINGS__SHORTCUT_SETTINGS_HPP__
