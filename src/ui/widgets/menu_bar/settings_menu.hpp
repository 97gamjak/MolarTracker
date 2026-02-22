#ifndef __UI__WIDGETS__MENU_BAR__SETTINGS_MENU_HPP__
#define __UI__WIDGETS__MENU_BAR__SETTINGS_MENU_HPP__

#include <QObject>

class QMenu;
class QMenuBar;
class QAction;

namespace ui
{
    /**
     * @brief Settings menu in the menu bar
     *
     */
    class SettingsMenu : public QObject
    {
        Q_OBJECT

       signals:
        /// Signal emitted when the user requests to open the preferences dialog
        void requestPreferences();

       private:
        /// Pointer to the settings menu widget
        QMenu* _settingsMenu = nullptr;

        /// The preferences action in the settings menu
        QAction* _preferencesAction = nullptr;

       public:
        explicit SettingsMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__WIDGETS__MENU_BAR__SETTINGS_MENU_HPP__