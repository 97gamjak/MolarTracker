#include "settings_menu.hpp"

#include <QMenuBar>

namespace ui
{
    /**
     * @brief Construct a new Settings Menu:: Settings Menu object
     *
     * @param menuBar
     */
    SettingsMenu::SettingsMenu(QMenuBar& menuBar)
    {
        _settingsMenu = menuBar.addMenu("&Settings");

        _preferencesAction = _settingsMenu->addAction("Preferences");

        connect(
            _preferencesAction,
            &QAction::triggered,
            this,
            &SettingsMenu::requestPreferences
        );
    }
}   // namespace ui