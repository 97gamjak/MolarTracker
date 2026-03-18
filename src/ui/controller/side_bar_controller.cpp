#include "side_bar_controller.hpp"

#include "ui/widgets/side_bar/account_category.hpp"
#include "ui/widgets/side_bar/account_item.hpp"
#include "ui/widgets/side_bar/overview_category.hpp"
#include "ui/widgets/side_bar/side_bar.hpp"
#include "ui/widgets/side_bar/side_bar_item.hpp"

namespace ui
{
    SideBarController::SideBarController(
        SideBar*        sideBar,
        QStackedWidget* centralStack
    )
        : _sideBar(sideBar), _centralStack(centralStack)
    {
        _accountCategory  = new AccountCategory();
        _overviewCategory = new OverviewCategory();

        _sideBar->addCategory(_overviewCategory);
        _sideBar->addCategory(_accountCategory);

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

    void SideBarController::refresh()
    {
        _accountCategory->clearAccounts();
        _accountCategory->addAccount(1, "Account 1");
        _accountCategory->addAccount(2, "Account 2");
    }

    void SideBarController::_onItemClicked(SideBarItem* item)
    {
        switch (item->getType())
        {
            case SideBarItemType::Accounts:
            {
                _openAccount(static_cast<AccountItem*>(item)->getId());
                break;
            }
            case SideBarItemType::Overview:
                // Handle overview item click
                break;
            case SideBarItemType::Category:
                // Handle category item click
                break;
            default:
                break;
        }
    }

    void SideBarController::_onContextMenuRequested(
        SideBarItem* item,
        QAction*     action
    )
    {
        switch (item->getType())
        {
            case SideBarItemType::Accounts:
            {
                auto* accountItem = static_cast<AccountItem*>(item);
                if (action == accountItem->getOpenAction())
                    _openAccount(accountItem->getId());
                else if (action == accountItem->getDeleteAction())
                {
                    //_deleteAccount(accountItem->getId());
                }
                break;
            }
            case SideBarItemType::Overview:
                // Handle overview item click
                break;
            case SideBarItemType::Category:
                // Handle category item click
                break;
            default:
                break;
        }
    }

    void SideBarController::_openAccount(int /*accountId*/) {}

}   // namespace ui
