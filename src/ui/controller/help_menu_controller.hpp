#ifndef __UI__CONTROLLER__HELP_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__HELP_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class HelpMenu;

    class HelpMenuController : public QObject
    {
        Q_OBJECT

       private:
        QMainWindow& _mainWindow;
        HelpMenu&    _helpMenu;

       private slots:
        void _onAboutRequested();

       public:
        explicit HelpMenuController(
            QMainWindow& mainWindow,
            HelpMenu&    helpMenu
        );
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__HELP_MENU_CONTROLLER_HPP__