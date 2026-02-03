#include "settings_menu.hpp"

#include <QMenuBar>

namespace ui
{
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