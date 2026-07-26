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
              ShortcutSettingsSchema::SHORTCUT_SETTINGS_KEY,
              ShortcutSettingsSchema::SHORTCUT_SETTINGS_TITLE,
              ShortcutSettingsSchema::SHORTCUT_SETTINGS_DESCRIPTION
          ),
          _shortcuts(
              ShortcutSettingsSchema::SHORTCUT_LIST_KEY,
              ShortcutSettingsSchema::SHORTCUT_LIST_TITLE,
              ShortcutSettingsSchema::SHORTCUT_LIST_DESCRIPTION
          )
    {
        _shortcuts.addValidator(
            [](const ParamCore<Shortcut>& shortcut,
               const ParamMap<Shortcut>&  params) -> ParamResult<void>
            {
                for (const auto& [key, storedShortcut] : params)
                {
                    if (storedShortcut.get().isKeyStrokeEqual(shortcut.get()))
                    {
                        return ParamError{
                            ParamErrorType::DuplicateValue,
                            "Duplicate shortcut found: " +
                                shortcut.get().getWhat()
                        };
                    }
                }
                return {};
            }
        );

        ParamCore<Shortcut> saveShortcut{
            ShortcutSettingsSchema::SAVE_SHORTCUT_KEY,
            ShortcutSettingsSchema::SAVE_SHORTCUT_TITLE,
            ShortcutSettingsSchema::SAVE_SHORTCUT_DESCRIPTION
        };

        saveShortcut.setDefault(
            Shortcut{
                std::get<0>(ShortcutSettingsSchema::SAVE_SHORTCUT_DEFAULT),
                std::get<1>(ShortcutSettingsSchema::SAVE_SHORTCUT_DEFAULT),
                std::get<2>(ShortcutSettingsSchema::SAVE_SHORTCUT_DEFAULT)
            }
        );

        _shortcuts.addParam(saveShortcut);

        ParamCore<Shortcut> quitShortcut{
            ShortcutSettingsSchema::QUIT_SHORTCUT_KEY,
            ShortcutSettingsSchema::QUIT_SHORTCUT_TITLE,
            ShortcutSettingsSchema::QUIT_SHORTCUT_DESCRIPTION
        };

        quitShortcut.setDefault(
            Shortcut{
                std::get<0>(ShortcutSettingsSchema::QUIT_SHORTCUT_DEFAULT),
                std::get<1>(ShortcutSettingsSchema::QUIT_SHORTCUT_DEFAULT),
                std::get<2>(ShortcutSettingsSchema::QUIT_SHORTCUT_DEFAULT)
            }
        );

        _shortcuts.addParam(quitShortcut);
    }

    /**
     * @brief Returns the save shortcut from the shortcut settings.
     *
     * @return Shortcut The save shortcut.
     */
    Shortcut ShortcutSettings::getSaveShortcut() const
    {
        return _shortcuts.at(ShortcutSettingsSchema::SAVE_SHORTCUT_KEY).get();
    }

    /**
     * @brief Returns the quit shortcut from the shortcut settings.
     *
     * @return Shortcut The quit shortcut.
     */
    Shortcut ShortcutSettings::getQuitShortcut() const
    {
        return _shortcuts.at(ShortcutSettingsSchema::QUIT_SHORTCUT_KEY).get();
    }

}   // namespace settings