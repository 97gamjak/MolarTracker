#ifndef __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__

#include <QObject>
#include <filesystem>
#include <functional>

class QMainWindow;   // Forward declaration

namespace ui
{
    class SettingsMenu;   // Forward declaration
}   // namespace ui

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace controller
{
    /**
     * @brief Controller for the settings menu actions
     *
     */
    class SettingsMenuController : public QObject
    {
        Q_OBJECT

       public:
        /// Callback type invoked with the chosen backup path when the user
        /// confirms a restore
        using RestoreCallback =
            std::function<void(const std::filesystem::path&)>;

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the settings menu
        ui::SettingsMenu& _settingsMenu;
        /// Reference to the application settings
        settings::Settings& _settings;
        /// Optional callback wired from MainController for restore
        RestoreCallback _restoreCallback;

       public:
        void setRestoreCallback(RestoreCallback callback);

       private slots:
        void _onPreferencesRequested();
        void _onRestoreFromBackupRequested();

       public:
        explicit SettingsMenuController(
            QMainWindow&        mainWindow,
            ui::SettingsMenu&   settingsMenu,
            settings::Settings& settings
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__