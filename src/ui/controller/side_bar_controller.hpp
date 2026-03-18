#ifndef __UI__CONTROLLER__SIDE_BAR_CONTROLLER_HPP__
#define __UI__CONTROLLER__SIDE_BAR_CONTROLLER_HPP__

#include <QObject>

class QStackedWidget;   // Forward declaration
class QAction;          // Forward declaration

namespace ui
{
    class SideBar;            // Forward declaration
    class AccountCategory;    // Forward declaration
    class OverviewCategory;   // Forward declaration
    class SideBarItem;        // Forward declaration

    class SideBarController : public QObject
    {
        Q_OBJECT

       private:
        SideBar*        _sideBar;
        QStackedWidget* _centralStack;

        AccountCategory*  _accountCategory;
        OverviewCategory* _overviewCategory;

       public:
        explicit SideBarController(
            SideBar*        sideBar,
            QStackedWidget* centralStack
        );

        void refresh();

       private:
        void _onItemClicked(SideBarItem* item);
        void _onContextMenuRequested(SideBarItem* item, QAction* action);

        void _openAccount(int accountId);
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__SIDE_BAR_CONTROLLER_HPP__