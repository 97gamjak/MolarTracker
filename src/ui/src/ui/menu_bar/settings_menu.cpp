#include "ui/menu_bar/settings_menu.hpp"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace ui
{
    /**
     * @brief Construct a new Settings Menu:: Settings Menu object
     *
     * @param menuBar
     */
    SettingsMenu::SettingsMenu(QMenuBar& menuBar) : QObject{&menuBar}
    {
        _settingsMenu = menuBar.addMenu("&Settings");

        _preferencesAction = _settingsMenu->addAction("&Preferences");

        connect(
            _preferencesAction,
            &QAction::triggered,
            this,
            &SettingsMenu::requestPreferences
        );

        _restoreBackupAction =
            _settingsMenu->addAction("&Restore from Backup…");

        connect(
            _restoreBackupAction,
            &QAction::triggered,
            this,
            &SettingsMenu::requestRestoreFromBackup
        );
    }
}   // namespace ui