#include "side_bar_controller.hpp"

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
     * @param sideBar
     * @param centralStack
     */
    SideBarController::SideBarController(
        SideBar*        sideBar,
        QStackedWidget* centralStack
    )
        : _sideBar(sideBar),
          _centralStack(centralStack),
          _accountSideBarController(
              std::make_unique<AccountSideBarController>()
          ),
          _overviewCategory(new OverviewCategory())
    {
        _sideBar->addCategory(_overviewCategory);
        _sideBar->addCategory(_accountSideBarController->getCategory());

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
    void SideBarController::refresh() { _accountSideBarController->refresh(); }

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
                _openAccount(dynamic_cast<AccountItem*>(item)->getId());
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
                    _openAccount(accountItem->getId());
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
                AccountSideBarController::handleContextMenuAction(
                    accountCategory,
                    action
                );
                break;
        }
    }

    /**
     * @brief Open the account page for the account with the given id, this will
     * switch the central stack to the account page and display the information
     * for the account with the given id
     *
     * @param accountId The id of the account to open, this should be used to
     * determine which account's information to display on the account page
     */
    void SideBarController::_openAccount(int /*accountId*/) {}

}   // namespace ui
