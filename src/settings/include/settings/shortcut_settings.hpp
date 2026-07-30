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

        static constexpr std::tuple<ShortcutModifier, std::int64_t>
            SHORTCUT_DEFAULT = {ShortcutModifier::None, 0};

        /// save shortcut key
        static constexpr const char* SAVE_SHORTCUT_KEY = "saveShortcut";
        /// save shortcut title
        static constexpr const char* SAVE_SHORTCUT_TITLE = "Save Shortcut";
        /// save shortcut description
        static constexpr const char* SAVE_SHORTCUT_DESCRIPTION =
            "The keyboard shortcut for saving the current state of the "
            "application.";
        /// default save shortcut value
        static constexpr std::
            tuple<std::string, ShortcutContext, ShortcutModifier, char>
                SAVE_SHORTCUT_DEFAULT = {
                    "Save",
                    ShortcutContext::Default,
                    ShortcutModifier::Control,
                    'S'
        };

        /// quit shortcut key
        static constexpr const char* QUIT_SHORTCUT_KEY = "quitShortcut";
        /// quit shortcut title
        static constexpr const char* QUIT_SHORTCUT_TITLE = "Quit Shortcut";
        /// quit shortcut description
        static constexpr const char* QUIT_SHORTCUT_DESCRIPTION =
            "The keyboard shortcut for quitting the application.";
        /// default quit shortcut value
        static constexpr std::
            tuple<std::string, ShortcutContext, ShortcutModifier, char>
                QUIT_SHORTCUT_DEFAULT = {
                    "Quit",
                    ShortcutContext::Default,
                    ShortcutModifier::Control,
                    'Q'
        };

        /// screenshot shortcut key
        static constexpr const char* SCREENSHOT_SHORTCUT_KEY =
            "screenshotShortcut";
        /// screenshot shortcut title
        static constexpr const char* SCREENSHOT_SHORTCUT_TITLE =
            "Screenshot Shortcut";
        /// screenshot shortcut description
        static constexpr const char* SCREENSHOT_SHORTCUT_DESCRIPTION =
            "The keyboard shortcut for taking a screenshot of the application.";
        /// default screenshot shortcut value
        static constexpr std::
            tuple<std::string, ShortcutContext, ShortcutModifier, FKey>
                SCREENSHOT_SHORTCUT_DEFAULT = {
                    "Screenshot",
                    ShortcutContext::Global,
                    ShortcutModifier::None,
                    FKey::F12
        };
        /// alternative screenshot shortcut value
        static constexpr std::tuple<ShortcutModifier, char>
            SCREENSHOT_SHORTCUT_ALTERNATIVE = {
                ShortcutModifier::ControlAlt,
                'P'
        };
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
        MapParam<ShortcutSet> _shortcuts{
            Schema::SHORTCUT_LIST_KEY,
            Schema::SHORTCUT_LIST_TITLE,
            Schema::SHORTCUT_LIST_DESCRIPTION
        };

       public:
        ShortcutSettings();

        [[nodiscard]] ShortcutSet getSaveShortcut() const;
        [[nodiscard]] ShortcutSet getQuitShortcut() const;
        [[nodiscard]] ShortcutSet getScreenshotShortcut() const;

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
