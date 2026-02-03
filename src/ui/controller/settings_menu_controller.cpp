#include "settings_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>

#include "ui/menu_bar/settings_menu.hpp"

namespace ui
{

    /**
     * @brief Construct a new Settings Menu Controller:: Settings Menu
     * Controller object
     *
     * @param mainWindow
     * @param settingsMenu
     */
    SettingsMenuController::SettingsMenuController(
        QMainWindow&  mainWindow,
        SettingsMenu& settingsMenu
    )
        : QObject{&mainWindow},
          _mainWindow(mainWindow),
          _settingsMenu(settingsMenu)
    {
        connect(
            &_settingsMenu,
            &SettingsMenu::requestPreferences,
            this,
            &SettingsMenuController::_onPreferencesRequested
        );
    }

    /**
     * @brief Handle preferences request
     *
     */
    void SettingsMenuController::_onPreferencesRequested()
    {
        QMessageBox::information(
            &_mainWindow,
            "Preferences",
            "Preferences requested"
        );
    }

}   // namespace ui