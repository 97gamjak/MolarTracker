#ifndef __CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__
#define __CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class SettingsMenu;   // Forward declaration
}   // namespace ui

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

       private slots:
        void _onPreferencesRequested();

       public:
        explicit SettingsMenuController(
            QMainWindow&      mainWindow,
            ui::SettingsMenu& settingsMenu
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__MENU_BAR__SETTINGS_MENU_CONTROLLER_HPP__