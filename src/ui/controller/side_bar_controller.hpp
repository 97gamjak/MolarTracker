#ifndef __UI__CONTROLLER__SIDE_BAR_CONTROLLER_HPP__
#define __UI__CONTROLLER__SIDE_BAR_CONTROLLER_HPP__

#include <QObject>

#include "account_controller.hpp"

class QStackedWidget;   // Forward declaration
class QAction;          // Forward declaration
class QMainWindow;      // Forward declaration

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace ui
{
    class SideBar;            // Forward declaration
    class OverviewCategory;   // Forward declaration
    class SideBarItem;        // Forward declaration
    class UndoStack;          // Forward declaration

    /**
     * @brief Controller for the side bar, this is responsible for managing the
     * state of the side bar and its categories and items, and providing an
     * interface for refreshing the side bar when the underlying data changes
     *
     */
    class SideBarController : public QObject
    {
        Q_OBJECT

       private:
        /// Pointer to the side bar widget
        SideBar* _sideBar;
        /// Pointer to the central stacked widget
        QStackedWidget* _centralStack;

        /// Controller for the accounts category in the side bar
        AccountSideBarController _accountSideBarController;
        /// Pointer to the overview category in the side bar
        OverviewCategory* _overviewCategory;

       public:
        explicit SideBarController(
            UndoStack&       undoStack,
            app::AppContext& appContext,
            QMainWindow*     mainWindow,
            SideBar*         sideBar,
            QStackedWidget*  centralStack
        );

        void refresh();

       private:
        void _onItemClicked(SideBarItem* item);
        void _onContextMenuRequested(SideBarItem* item, QAction* action);
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__SIDE_BAR_CONTROLLER_HPP__