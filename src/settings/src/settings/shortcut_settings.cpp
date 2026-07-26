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
            [](const ParamCore<Shortcut>&              shortcut,
               const std::vector<ParamCore<Shortcut>>& params
            ) -> ParamResult<void>
            {
                for (const auto& storedShortcut : params)
                {
                    if (storedShortcut == shortcut)
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
    }

}   // namespace settings