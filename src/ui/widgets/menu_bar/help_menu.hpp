#ifndef __UI__MENU_BAR__HELP_MENU_HPP__
#define __UI__MENU_BAR__HELP_MENU_HPP__

#include <QObject>

class QMenu;
class QMenuBar;
class QAction;

namespace ui
{
    /**
     * @brief Help menu in the menu bar
     *
     */
    class HelpMenu : public QObject
    {
        Q_OBJECT

       signals:
        /// Signal emitted when the user requests to see the about dialog
        void requestAbout();

       private:
        /// Pointer to the help menu widget
        QMenu* _helpMenu = nullptr;

        /// The about action in the help menu
        QAction* _aboutAction = nullptr;

       public:
        explicit HelpMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__HELP_MENU_HPP__