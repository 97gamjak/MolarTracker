#ifndef __UI__CONTROLLER__SETTINGS_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__SETTINGS_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class SettingsMenu;   // Forward declaration

    /**
     * @brief Controller for the settings menu actions
     *
     */
    class SettingsMenuController : public QObject
    {
        Q_OBJECT

       private:
        QMainWindow&  _mainWindow;
        SettingsMenu& _settingsMenu;

       private slots:
        void _onPreferencesRequested();

       public:
        explicit SettingsMenuController(
            QMainWindow&  mainWindow,
            SettingsMenu& settingsMenu
        );
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__SETTINGS_MENU_CONTROLLER_HPP__