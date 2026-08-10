#include "side_bar_controller.hpp"

#include <qstackedwidget.h>

#include <QMainWindow>

#include "account_controller.hpp"
#include "controller/account_controller.hpp"
#include "logging/log_macros.hpp"
#include "store/store_container.hpp"
#include "ui/side_bar/account_category.hpp"
#include "ui/side_bar/account_item.hpp"
#include "ui/side_bar/overview_category.hpp"
#include "ui/side_bar/securities_category.hpp"
#include "ui/side_bar/side_bar.hpp"
#include "ui/side_bar/side_bar_item.hpp"
#include "ui/side_bar/transaction_category.hpp"
#include "ui/side_bar/watchlist_item.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.SideBarController");

namespace controller
{
    /**
     * @brief Construct a new Side Bar Controller:: Side Bar Controller object
     *
     * @param undoStack The undo stack for the application
     * @param storeContainer The store container for the application
     * @param mainWindow The main window of the application
     * @param sideBar
     * @param centralStack
     * @param accountController
     * @param transactionController
     * @param positionGateway
     */
    // TODO(97gamjak): would be probably best to remove dependency on central
    // stack here
    SideBarController::SideBarController(
        cmd::UndoStack&                                  undoStack,
        store::StoreContainer&                           storeContainer,
        QMainWindow*                                     mainWindow,
        ui::SideBar*                                     sideBar,
        QStackedWidget*                                  centralStack,
        AccountController&                               accountController,
        TransactionController&                           transactionController,
        const std::shared_ptr<gateway::PositionGateway>& positionGateway
    )
        : _sideBar(sideBar),
          _centralStack(centralStack),
          _accountSideBarController(
              undoStack,
              storeContainer.getAccountStore(),
              accountController,
              mainWindow
          ),
          _securitiesSideBarController(
              mainWindow,
              storeContainer.getStockStore(),
              storeContainer.getWatchlistStore(),
              centralStack
          ),
          _transactionSideBarController(
              undoStack,
              positionGateway,
              storeContainer.getAccountStore(),
              storeContainer.getTransactionStore(),
              storeContainer.getStockStore(),
              storeContainer.getOptionStore(),
              storeContainer.getPositionStore(),
              transactionController,
              _securitiesSideBarController,
              mainWindow
          ),
          _overviewCategory(new ui::OverviewCategory())
    {
        _sideBar->addCategory(_overviewCategory);
        _sideBar->addCategory(_accountSideBarController.getCategory());
        _sideBar->addCategory(_transactionSideBarController.getCategory());
        _sideBar->addCategory(_securitiesSideBarController.getCategory());

        connect(
            _sideBar,
            &ui::SideBar::itemClicked,
            this,
            [this](ui::SideBarItem* item)
            { _onActionTriggered(item, SideBarAction::ItemClicked, nullptr); }
        );

        connect(
            _sideBar,
            &ui::SideBar::itemDoubleClicked,
            this,
            [this](ui::SideBarItem* item)
            {
                _onActionTriggered(
                    item,
                    SideBarAction::ItemDoubleClicked,
                    nullptr
                );
            }
        );

        connect(
            _sideBar,
            &ui::SideBar::contextMenuRequested,
            this,
            [this](ui::SideBarItem* item, QAction* action)
            {
                _onActionTriggered(
                    item,
                    SideBarAction::ContextMenuRequested,
                    action
                );
            }
        );

        connect(
            _sideBar,
            &ui::SideBar::itemRenameCommitted,
            this,
            [this](ui::SideBarItem* item, const QString& newName)
            {
                if (item->getType() == ui::SideBarItemType::AccountsItem)
                {
                    auto* accountItem = dynamic_cast<ui::AccountItem*>(item);
                    _accountSideBarController.renameAccount(
                        accountItem,
                        newName
                    );
                }
            }
        );

        refresh();
    }

    /**
     * @brief Refresh the side bar, this should update the items in the side bar
     * to reflect the current state of the underlying data, this will be called
     * when the underlying data changes and the side bar needs to be updated to
     * reflect those changes
     *
     */
    void SideBarController::refresh()
    {
        _accountSideBarController.refresh();
        _transactionSideBarController.refresh();
        _securitiesSideBarController.refresh();
    }

    /**
     * @brief Handle an action being triggered for an item in the side bar, this
     * will determine which item the action was triggered for and which action
     * was triggered, and perform the appropriate action based on that
     * information
     *
     * @param item The item for which the action was triggered, this should be a
     * pointer to a SideBarItem that is currently in the side bar, and will be
     * used to determine which item the action was triggered for
     * @param action The action that was triggered, this should be a value from
     * the SideBarAction enum that indicates which action was triggered, and
     * will be used to determine which action to perform
     * @param qaction The QAction that was triggered, this should be a pointer
     * to a QAction that is currently in the context menu for the item, and will
     * be used to determine which action was triggered, this will be nullptr if
     * the action was not triggered from a context menu (e.g. if it was
     * triggered by a click or double click)
     */
    void SideBarController::_onActionTriggered(
        ui::SideBarItem* item,
        SideBarAction    action,
        QAction*         qaction
    )
    {
        if (item == nullptr)
        {
            LOG_WARNING(
                "Context menu action triggered with null item, ignoring"
            );
            return;
        }

        switch (item->getType())
        {
            case ui::SideBarItemType::AccountsItem:
            case ui::SideBarItemType::AccountCategory:
            {
                _accountSideBarController
                    .handleTriggeredAction(item, action, qaction);
                break;
            }
            case ui::SideBarItemType::TransactionCategory:
            {
                _transactionSideBarController
                    .handleTriggeredAction(item, action, qaction);
                break;
            }
            case ui::SideBarItemType::SecuritiesCategory:
            case ui::SideBarItemType::WatchlistItem:
            case ui::SideBarItemType::AllSecuritiesItem:
            {
                _securitiesSideBarController
                    .handleTriggeredAction(item, action, qaction);
                break;
            }
            case ui::SideBarItemType::OverviewCategory:
                // Handle overview and all securities item click if needed
                break;
        }
    }

}   // namespace controller
