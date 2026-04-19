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

       private slots:
        void _onPreferencesRequested();

       public:
        explicit SettingsMenuController(
            QMainWindow&        mainWindow,
            ui::SettingsMenu&   settingsMenu,
            settings::Settings& settings
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__