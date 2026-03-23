#include "side_bar_controller.hpp"

#include <QMainWindow>

#include "account_controller.hpp"
#include "logging/log_macros.hpp"
#include "ui/widgets/side_bar/account_category.hpp"
#include "ui/widgets/side_bar/account_item.hpp"
#include "ui/widgets/side_bar/overview_category.hpp"
#include "ui/widgets/side_bar/side_bar.hpp"
#include "ui/widgets/side_bar/side_bar_item.hpp"

REGISTER_LOG_CATEGORY("UI.Controller.SideBarController");

namespace ui
{
    /**
     * @brief Construct a new Side Bar Controller:: Side Bar Controller object
     *
     * @param undoStack The undo stack for the application
     * @param appContext The application context
     * @param mainWindow The main window of the application
     * @param sideBar
     * @param centralStack
     */
    SideBarController::SideBarController(
        UndoStack&       undoStack,
        app::AppContext& appContext,
        QMainWindow*     mainWindow,
        SideBar*         sideBar,
        QStackedWidget*  centralStack
    )
        : _sideBar(sideBar),
          _centralStack(centralStack),
          _accountSideBarController(undoStack, appContext, mainWindow),
          _overviewCategory(new OverviewCategory())
    {
        _sideBar->addCategory(_overviewCategory);
        _sideBar->addCategory(_accountSideBarController.getCategory());

        connect(
            _sideBar,
            &SideBar::itemClicked,
            this,
            &SideBarController::_onItemClicked
        );

        connect(
            _sideBar,
            &SideBar::contextMenuRequested,
            this,
            &SideBarController::_onContextMenuRequested
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
    void SideBarController::refresh() { _accountSideBarController.refresh(); }

    /**
     * @brief Handle an item being clicked in the side bar, this will determine
     * which item was clicked and perform the appropriate action, such as
     * opening the corresponding page in the central stack
     *
     * @param item The item that was clicked, this should be a pointer to a
     * SideBarItem that is currently in the side bar, and will be used to
     * determine which item was clicked and what action to perform
     */
    void SideBarController::_onItemClicked(SideBarItem* item)
    {
        switch (item->getType())
        {
            case SideBarItemType::AccountsItem:
            {
                // open the account page for the selected account
                break;
            }
            case SideBarItemType::OverviewCategory:
            case SideBarItemType::AccountCategory:
                break;
        }
    }

    /**
     * @brief Handle a context menu action being triggered for an item in the
     * side bar, this will determine which item the action was triggered for and
     * which action was triggered, and perform the appropriate action based on
     * that information
     *
     * @param item The item for which the context menu action was triggered,
     * this should be a pointer to a SideBarItem that is currently in the side
     * bar, and will be used to determine which item the action was triggered
     * for
     * @param action The action that was triggered, this should be a pointer to
     * a QAction that is currently in the context menu for the item, and will be
     * used to determine which action was triggered
     */
    void SideBarController::_onContextMenuRequested(
        SideBarItem* item,
        QAction*     action
    )
    {
        if (item == nullptr || action == nullptr)
        {
            LOG_WARNING(
                "Context menu requested with null item or action, ignoring"
            );
            return;
        }

        switch (item->getType())
        {
            case SideBarItemType::AccountsItem:
            {
                const auto* accountItem = dynamic_cast<AccountItem*>(item);
                if (action == accountItem->getOpenAction())
                // NOLINTNEXTLINE(bugprone-branch-clone)
                {
                    // _openAccount(accountItem->getId());
                }
                else if (action == accountItem->getDeleteAction())
                {
                    //_deleteAccount(accountItem->getId());
                }
                break;
            }
            case SideBarItemType::OverviewCategory:
                // Handle overview item click
                break;
            case SideBarItemType::AccountCategory:
                const auto* accountCategory =
                    dynamic_cast<AccountCategory*>(item);
                _accountSideBarController.handleContextMenuAction(
                    accountCategory,
                    action
                );
                break;
        }
    }

}   // namespace ui
