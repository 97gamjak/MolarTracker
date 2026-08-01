#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QString>

#include "config/id_types.hpp"
#include "ui/side_bar/account_category.hpp"
#include "ui/side_bar/account_item.hpp"
#include "ui/side_bar/all_securities_item.hpp"
#include "ui/side_bar/securities_category.hpp"
#include "ui/side_bar/side_bar_item.hpp"
#include "ui/side_bar/watchlist_item.hpp"

namespace
{
    // -------------------------------------------------------------------------
    // AccountItem
    // -------------------------------------------------------------------------

    class AccountItemTest : public ::testing::Test
    {
    };

    TEST_F(AccountItemTest, GetTypeReturnsAccountsItem)
    {
        ui::AccountItem item{AccountId{1}, "Savings"};
        EXPECT_EQ(item.getType(), ui::SideBarItemType::AccountsItem);
    }

    TEST_F(AccountItemTest, GetIdReturnsGivenId)
    {
        const auto      id = 42;
        ui::AccountItem item{AccountId{id}, "Savings"};
        EXPECT_EQ(item.getId(), AccountId{id});
    }

    TEST_F(AccountItemTest, GetIdDifferentiatesBetweenItems)
    {
        ui::AccountItem item1{AccountId{1}, "Alpha"};
        ui::AccountItem item2{AccountId{2}, "Beta"};
        EXPECT_NE(item1.getId(), item2.getId());
    }

    TEST_F(AccountItemTest, ActionsAreNullBeforePopulatingContextMenu)
    {
        ui::AccountItem item{AccountId{1}, "Savings"};
        EXPECT_EQ(item.getOpenAction(), nullptr);
        EXPECT_EQ(item.getDeleteAction(), nullptr);
    }

    TEST_F(AccountItemTest, PopulateContextMenuCreatesOpenAndDeleteActions)
    {
        ui::AccountItem item{AccountId{1}, "Savings"};
        QMenu           menu;
        item.populateContextMenu(menu);

        EXPECT_NE(item.getOpenAction(), nullptr);
        EXPECT_NE(item.getDeleteAction(), nullptr);
    }

    TEST_F(AccountItemTest, OpenActionTextIsOpen)
    {
        ui::AccountItem item{AccountId{1}, "Savings"};
        QMenu           menu;
        item.populateContextMenu(menu);

        EXPECT_EQ(item.getOpenAction()->text(), "Open");
    }

    TEST_F(AccountItemTest, DeleteActionTextIsDelete)
    {
        ui::AccountItem item{AccountId{1}, "Savings"};
        QMenu           menu;
        item.populateContextMenu(menu);

        EXPECT_EQ(item.getDeleteAction()->text(), "Delete");
    }

    TEST_F(AccountItemTest, OpenAndDeleteActionsAreDifferent)
    {
        ui::AccountItem item{AccountId{1}, "Savings"};
        QMenu           menu;
        item.populateContextMenu(menu);

        EXPECT_NE(item.getOpenAction(), item.getDeleteAction());
    }

    // -------------------------------------------------------------------------
    // AccountCategory
    // -------------------------------------------------------------------------

    class AccountCategoryTest : public ::testing::Test
    {
    };

    TEST_F(AccountCategoryTest, GetTypeReturnsAccountCategory)
    {
        ui::AccountCategory cat;
        EXPECT_EQ(cat.getType(), ui::SideBarItemType::AccountCategory);
    }

    TEST_F(AccountCategoryTest, InitiallyHasNoChildRows)
    {
        ui::AccountCategory cat;
        EXPECT_EQ(cat.rowCount(), 0);
    }

    TEST_F(AccountCategoryTest, AddAccountIncrementsRowCount)
    {
        ui::AccountCategory cat;
        cat.addAccount(AccountId{1}, "Savings", AccountKind::Cash);
        EXPECT_EQ(cat.rowCount(), 1);
    }

    TEST_F(AccountCategoryTest, AddMultipleAccountsIncrementRowCount)
    {
        ui::AccountCategory cat;
        cat.addAccount(AccountId{1}, "Savings", AccountKind::Cash);
        cat.addAccount(AccountId{2}, "Checking", AccountKind::Cash);
        EXPECT_EQ(cat.rowCount(), 2);
    }

    TEST_F(AccountCategoryTest, ClearAccountsResetsRowCount)
    {
        ui::AccountCategory cat;
        cat.addAccount(AccountId{1}, "Savings", AccountKind::Cash);
        cat.addAccount(AccountId{2}, "Checking", AccountKind::Cash);
        cat.clearAccounts();
        EXPECT_EQ(cat.rowCount(), 0);
    }

    TEST_F(AccountCategoryTest, ClearOnEmptyCategoryIsNoop)
    {
        ui::AccountCategory cat;
        EXPECT_NO_THROW(cat.clearAccounts());
        EXPECT_EQ(cat.rowCount(), 0);
    }

    TEST_F(AccountCategoryTest, CreateActionNullBeforePopulatingContextMenu)
    {
        ui::AccountCategory cat;
        EXPECT_EQ(cat.getCreateAction(), nullptr);
    }

    TEST_F(AccountCategoryTest, PopulateContextMenuCreatesAction)
    {
        ui::AccountCategory cat;
        QMenu               menu;
        cat.populateContextMenu(menu);
        EXPECT_NE(cat.getCreateAction(), nullptr);
    }

    TEST_F(AccountCategoryTest, CreateActionTextMentionsAccount)
    {
        ui::AccountCategory cat;
        QMenu               menu;
        cat.populateContextMenu(menu);
        EXPECT_TRUE(cat.getCreateAction()->text().contains(
            "Account",
            Qt::CaseInsensitive
        ));
    }

    // -------------------------------------------------------------------------
    // AllSecuritiesItem
    // -------------------------------------------------------------------------

    class AllSecuritiesItemTest : public ::testing::Test
    {
    };

    TEST_F(AllSecuritiesItemTest, GetTypeReturnsAllSecuritiesItem)
    {
        ui::AllSecuritiesItem item;
        EXPECT_EQ(item.getType(), ui::SideBarItemType::AllSecuritiesItem);
    }

    TEST_F(AllSecuritiesItemTest, PopulateContextMenuAddsNoActions)
    {
        ui::AllSecuritiesItem item;
        QMenu                 menu;
        item.populateContextMenu(menu);
        EXPECT_TRUE(menu.actions().isEmpty());
    }

    // -------------------------------------------------------------------------
    // WatchlistItem
    // -------------------------------------------------------------------------

    class WatchlistItemTest : public ::testing::Test
    {
    };

    TEST_F(WatchlistItemTest, GetTypeReturnsWatchlistItem)
    {
        ui::WatchlistItem item{WatchlistId{1}, "Tech Stocks"};
        EXPECT_EQ(item.getType(), ui::SideBarItemType::WatchlistItem);
    }

    TEST_F(WatchlistItemTest, GetIdReturnsGivenId)
    {
        constexpr auto    id = 42;
        ui::WatchlistItem item{WatchlistId{id}, "Tech Stocks"};
        EXPECT_EQ(item.getId(), WatchlistId{id});
    }

    TEST_F(WatchlistItemTest, ActionsAreNullBeforePopulatingContextMenu)
    {
        ui::WatchlistItem item{WatchlistId{1}, "Tech Stocks"};
        EXPECT_EQ(item.getRenameAction(), nullptr);
        EXPECT_EQ(item.getDeleteAction(), nullptr);
    }

    TEST_F(WatchlistItemTest, PopulateContextMenuCreatesRenameAndDeleteActions)
    {
        ui::WatchlistItem item{WatchlistId{1}, "Tech Stocks"};
        QMenu             menu;
        item.populateContextMenu(menu);

        EXPECT_NE(item.getRenameAction(), nullptr);
        EXPECT_NE(item.getDeleteAction(), nullptr);
    }

    // -------------------------------------------------------------------------
    // SecuritiesCategory
    // -------------------------------------------------------------------------

    class SecuritiesCategoryTest : public ::testing::Test
    {
    };

    TEST_F(SecuritiesCategoryTest, GetTypeReturnsSecuritiesCategory)
    {
        ui::SecuritiesCategory cat;
        EXPECT_EQ(cat.getType(), ui::SideBarItemType::SecuritiesCategory);
    }

    TEST_F(SecuritiesCategoryTest, HasPermanentAllSecuritiesRowAtConstruction)
    {
        ui::SecuritiesCategory cat;
        EXPECT_EQ(cat.rowCount(), 1);
        EXPECT_NE(cat.getAllSecuritiesItem(), nullptr);
    }

    TEST_F(SecuritiesCategoryTest, AddWatchlistIncrementsRowCount)
    {
        ui::SecuritiesCategory cat;
        cat.addWatchlist(WatchlistId{1}, "Tech Stocks");
        EXPECT_EQ(cat.rowCount(), 2);
    }

    TEST_F(SecuritiesCategoryTest, ClearWatchlistsPreservesAllSecuritiesRow)
    {
        ui::SecuritiesCategory cat;
        cat.addWatchlist(WatchlistId{1}, "Tech Stocks");
        cat.addWatchlist(WatchlistId{2}, "Dividend Plays");
        cat.clearWatchlists();

        EXPECT_EQ(cat.rowCount(), 1);
        EXPECT_NE(cat.getAllSecuritiesItem(), nullptr);
    }

    TEST_F(SecuritiesCategoryTest, PopulateContextMenuCreatesBothActions)
    {
        ui::SecuritiesCategory cat;
        QMenu                  menu;
        cat.populateContextMenu(menu);

        EXPECT_NE(cat.getCreateAction(), nullptr);
        EXPECT_NE(cat.getCreateWatchlistAction(), nullptr);
    }

}   // namespace
