#ifndef __UI__CONTROLLER__HELP_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__HELP_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class HelpMenu;

    /**
     * @brief Controller for the help menu actions
     *
     */
    class HelpMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the help menu
        HelpMenu& _helpMenu;

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