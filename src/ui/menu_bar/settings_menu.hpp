#ifndef __UI__MENU_BAR__SETTINGS_MENU_HPP__
#define __UI__MENU_BAR__SETTINGS_MENU_HPP__

#include <QObject>

class QMenu;
class QMenuBar;
class QAction;

namespace ui
{
    class SettingsMenu : public QObject
    {
        Q_OBJECT

       signals:
        void requestPreferences();

       private:
        QMenu* _settingsMenu = nullptr;

        QAction* _preferencesAction = nullptr;

       public:
        explicit SettingsMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__SETTINGS_MENU_HPP__