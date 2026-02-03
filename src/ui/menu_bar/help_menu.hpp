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
        void requestAbout();

       private:
        QMenu* _helpMenu = nullptr;

        QAction* _aboutAction = nullptr;

       public:
        explicit HelpMenu(QMenuBar& menuBar);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__HELP_MENU_HPP__