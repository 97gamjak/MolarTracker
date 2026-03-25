#ifndef __CONTROLLER__MENU_BAR__HELP_MENU_CONTROLLER_HPP__
#define __CONTROLLER__MENU_BAR__HELP_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class HelpMenu;   // Forward declaration
}   // namespace ui

namespace controller
{
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
        ui::HelpMenu& _helpMenu;

       private slots:
        void _onAboutRequested();

       public:
        explicit HelpMenuController(
            QMainWindow&  mainWindow,
            ui::HelpMenu& helpMenu
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__MENU_BAR__HELP_MENU_CONTROLLER_HPP__