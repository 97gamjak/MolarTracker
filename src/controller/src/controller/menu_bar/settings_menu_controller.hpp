#ifndef __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class SettingsMenu;   // Forward declaration
}   // namespace ui

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace store
{
    class StoreContainer;   // Forward declaration
}   // namespace store

namespace controller
{
    /**
     * @brief Controller for the settings menu actions
     *
     */
    class SettingsMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the settings menu
        ui::SettingsMenu& _settingsMenu;
        /// Reference to the application settings
        settings::Settings& _settings;
        /// Reference to the store container for managing application state
        store::StoreContainer& _storeContainer;

       private slots:
        void _onPreferencesRequested();
        void _onRestoreFromBackupRequested();

       public:
        explicit SettingsMenuController(
            QMainWindow&           mainWindow,
            ui::SettingsMenu&      settingsMenu,
            settings::Settings&    settings,
            store::StoreContainer& storeContainer
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__