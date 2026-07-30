#include "settings/shortcut_settings.hpp"

#include "error/param_error.hpp"

namespace settings
{
    /**
     * @brief Construct a new ShortcutSettings::ShortcutSettings object
     *
     */
    ShortcutSettings::ShortcutSettings()
        : _core(
              Schema::SHORTCUT_SETTINGS_KEY,
              Schema::SHORTCUT_SETTINGS_TITLE,
              Schema::SHORTCUT_SETTINGS_DESCRIPTION
          ),
          _shortcuts(
              Schema::SHORTCUT_LIST_KEY,
              Schema::SHORTCUT_LIST_TITLE,
              Schema::SHORTCUT_LIST_DESCRIPTION
          )
    {
        const auto defaultShortcut = Shortcut{
            std::get<0>(Schema::SHORTCUT_DEFAULT),
            std::get<1>(Schema::SHORTCUT_DEFAULT)
        };

        _shortcuts.addValidator(
            [defaultShortcut](
                const ParamCore<ShortcutSet>& shortcut,
                const ParamMap<ShortcutSet>&  params
            ) -> ParamResult<void>
            {
                for (const auto& [key, storedShortcut] : params)
                {
                    for (const auto& oldShortcut :
                         storedShortcut.get().getShortcuts())
                    {
                        for (const auto& newShortcut :
                             shortcut.get().getShortcuts())
                        {
                            // Ignore the default shortcut — it is always
                            // allowed to be duplicated, since it is the "no
                            // shortcut" value.
                            if (newShortcut == defaultShortcut)
                                continue;

                            if (oldShortcut == newShortcut)
                            {
                                return ParamError{
                                    ParamErrorType::DuplicateValue,
                                    "Duplicate shortcut found for new "
                                    "shortcut: " +
                                        newShortcut.toString() + " in " +
                                        shortcut.get().getWhat() + " and " +
                                        storedShortcut.get().getWhat()
                                };
                            }
                        }
                    }
                }
                return {};
            }
        );

        // save shortcut

        ParamCore<ShortcutSet> saveShortcut{
            Schema::SAVE_SHORTCUT_KEY,
            Schema::SAVE_SHORTCUT_TITLE,
            Schema::SAVE_SHORTCUT_DESCRIPTION
        };

        saveShortcut.setDefault(
            ShortcutSet{
                std::get<0>(Schema::SAVE_SHORTCUT_DEFAULT),
                std::get<1>(Schema::SAVE_SHORTCUT_DEFAULT),
                Shortcut{
                    std::get<2>(Schema::SAVE_SHORTCUT_DEFAULT),
                    std::get<3>(Schema::SAVE_SHORTCUT_DEFAULT)
                }
            }
        );

        _shortcuts.addParam(saveShortcut);

        // quit shortcut

        ParamCore<ShortcutSet> quitShortcut{
            Schema::QUIT_SHORTCUT_KEY,
            Schema::QUIT_SHORTCUT_TITLE,
            Schema::QUIT_SHORTCUT_DESCRIPTION
        };

        quitShortcut.setDefault(
            ShortcutSet{
                std::get<0>(Schema::QUIT_SHORTCUT_DEFAULT),
                std::get<1>(Schema::QUIT_SHORTCUT_DEFAULT),
                Shortcut{
                    std::get<2>(Schema::QUIT_SHORTCUT_DEFAULT),
                    std::get<3>(Schema::QUIT_SHORTCUT_DEFAULT)
                }
            }
        );

        _shortcuts.addParam(quitShortcut);

        // screenshot shortcut
        ParamCore<ShortcutSet> screenshotShortcut{
            Schema::SCREENSHOT_SHORTCUT_KEY,
            Schema::SCREENSHOT_SHORTCUT_TITLE,
            Schema::SCREENSHOT_SHORTCUT_DESCRIPTION
        };
        auto defaultScreenshotShortcut = ShortcutSet{
            std::get<0>(Schema::SCREENSHOT_SHORTCUT_DEFAULT),
            std::get<1>(Schema::SCREENSHOT_SHORTCUT_DEFAULT),
            Shortcut{
                std::get<2>(Schema::SCREENSHOT_SHORTCUT_DEFAULT),
                std::get<3>(Schema::SCREENSHOT_SHORTCUT_DEFAULT)
            }
        };
        defaultScreenshotShortcut.addAlternativeShortcut(
            Shortcut{
                std::get<0>(Schema::SCREENSHOT_SHORTCUT_ALTERNATIVE),
                std::get<1>(Schema::SCREENSHOT_SHORTCUT_ALTERNATIVE)
            }
        );

        screenshotShortcut.setDefault(defaultScreenshotShortcut);

        _shortcuts.addParam(screenshotShortcut);
    }

    /**
     * @brief Returns the save shortcut from the shortcut settings.
     *
     * @return ShortcutSet The save shortcut.
     */
    ShortcutSet ShortcutSettings::getSaveShortcut() const
    {
        return _shortcuts.at(Schema::SAVE_SHORTCUT_KEY).get();
    }

    /**
     * @brief Returns the quit shortcut from the shortcut settings.
     *
     * @return ShortcutSet The quit shortcut.
     */
    ShortcutSet ShortcutSettings::getQuitShortcut() const
    {
        return _shortcuts.at(Schema::QUIT_SHORTCUT_KEY).get();
    }

    /**
     * @brief Returns the screenshot shortcut from the shortcut settings.
     *
     * @return ShortcutSet The screenshot shortcut.
     */
    ShortcutSet ShortcutSettings::getScreenshotShortcut() const
    {
        return _shortcuts.at(Schema::SCREENSHOT_SHORTCUT_KEY).get();
    }

}   // namespace settings
