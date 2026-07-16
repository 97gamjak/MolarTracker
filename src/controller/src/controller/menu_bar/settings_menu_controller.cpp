#include "settings_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>

#include "db/backup_manager.hpp"
#include "settings/settings.hpp"
#include "store/store_container.hpp"
#include "ui/backup/restore_backup_dialog.hpp"
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
     * @param settings
     */
    SettingsMenuController::SettingsMenuController(
        QMainWindow&           mainWindow,
        ui::SettingsMenu&      settingsMenu,
        settings::Settings&    settings,
        store::StoreContainer& storeContainer
    )
        : QObject{&mainWindow},
          _mainWindow(mainWindow),
          _settingsMenu(settingsMenu),
          _settings(settings),
          _storeContainer(storeContainer)
    {
        connect(
            &_settingsMenu,
            &ui::SettingsMenu::requestPreferences,
            this,
            &SettingsMenuController::_onPreferencesRequested
        );

        connect(
            &_settingsMenu,
            &ui::SettingsMenu::requestRestoreFromBackup,
            this,
            &SettingsMenuController::_onRestoreFromBackupRequested
        );
    }

    /**
     * @brief Show the restore-from-backup dialog. If the user confirms, invoke
     * the restore callback wired from MainController.
     */
    void SettingsMenuController::_onRestoreFromBackupRequested()
    {
        const auto backups =
            db::BackupManager::listBackups(_settings.getBackupSettings());

        if (backups.empty())
        {
            QMessageBox::information(
                &_mainWindow,
                "No Backups",
                "No backup files were found in the backup directory."
            );
            return;
        }

        auto* dialog =
            utils::makeQChild<ui::RestoreBackupDialog>(backups, &_mainWindow);

        if (dialog->exec() == QDialog::Accepted && dialog->selectedBackup())
        {
            _restoreFromBackup(*dialog->selectedBackup());
        }
    }

    /**
     * @brief Handle preferences request
     *
     */
    void SettingsMenuController::_onPreferencesRequested()
    {
        _settings.save();

        auto* settingsDialog =
            utils::makeQChild<ui::SettingsDialog>(_settings, &_mainWindow);

        const auto snapShot = _settings.toJson();

        connect(
            settingsDialog,
            &ui::SettingsDialog::saveRequested,
            this,
            [this]() { _settings.save(); }
        );

        if (settingsDialog->exec() != QDialog::Accepted)
        {
            // User cancelled, revert any changes
            settings::Settings::fromJson(snapShot, _settings);
            _settings.save();
        }
    }

    void SettingsMenuController::_restoreFromBackup(
        const std::filesystem::path& backupFile
    )
    {
        _storeContainer.restoreFromBackup(backupFile);
        // TODO(97gamjak): restart app
    }

}   // namespace controller