#include "settings_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>

#include "app/app_context.hpp"
#include "ui/menu_bar/settings_menu.hpp"
#include "ui/settings/settings_dialog.hpp"
#include "utils/qt_helpers.hpp"

namespace controller
{

    /**
     * @brief Construct a new Settings Menu Controller:: Settings Menu
     * Controller object
     *
     * @param mainWindow
     * @param settingsMenu
     */
    SettingsMenuController::SettingsMenuController(
        QMainWindow&        mainWindow,
        ui::SettingsMenu&   settingsMenu,
        settings::Settings& settings
    )
        : QObject{&mainWindow},
          _mainWindow(mainWindow),
          _settingsMenu(settingsMenu),
          _settings(settings)
    {
        connect(
            &_settingsMenu,
            &ui::SettingsMenu::requestPreferences,
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
        auto* settingsDialog =
            utils::makeQChild<ui::SettingsDialog>(_settings, &_mainWindow);

        const auto snapShot = _settings.toJson();

        if (settingsDialog->exec() != QDialog::Accepted)
        {
            // User cancelled, revert any changes
            settings::Settings::fromJson(snapShot, _settings);
            _settings.commit();
        }
    }

}   // namespace controller