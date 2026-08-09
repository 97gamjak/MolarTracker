#ifndef __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_CONTROLLER_HPP__

#include <QObject>

#include "account_controller.hpp"
#include "securities_controller.hpp"
#include "side_bar_action.hpp"
#include "transaction_controller.hpp"

class QStackedWidget;   // Forward declaration
class QAction;          // Forward declaration
class QMainWindow;      // Forward declaration

namespace store
{
    class StoreContainer;   // Forward declaration
}   // namespace store

namespace ui
{
    class SideBar;            // Forward declaration
    class OverviewCategory;   // Forward declaration
    class SideBarItem;        // Forward declaration
}   // namespace ui

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace gateway
{
    class PositionGateway;   // Forward declaration
}   // namespace gateway

namespace controller
{

    class AccountController;       // Forward declaration
    class TransactionController;   // Forward declaration
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
        ui::SideBar* _sideBar;
        /// Pointer to the central stacked widget
        QStackedWidget* _centralStack;

        /// Controller for the accounts category in the side bar
        AccountSideBarController _accountSideBarController;
        /// Controller for the securities category in the side bar
        SecuritiesSideBarController _securitiesSideBarController;
        /// Controller for the transactions category in the side bar
        TransactionSideBarController _transactionSideBarController;
        /// Pointer to the overview category in the side bar
        ui::OverviewCategory* _overviewCategory;

       public:
        explicit SideBarController(
            cmd::UndoStack&        undoStack,
            store::StoreContainer& storeContainer,
            QMainWindow*           mainWindow,
            ui::SideBar*           sideBar,
            QStackedWidget*        centralStack,
            AccountController&     accountController,
            TransactionController& transactionController,
            const std::shared_ptr<gateway::PositionGateway>& positionGateway
        );

        void refresh();

       private:
        void _onActionTriggered(
            ui::SideBarItem* item,
            SideBarAction    action,
            QAction*         qaction = nullptr
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__SIDE_BAR__SIDE_BAR_CONTROLLER_HPP__
