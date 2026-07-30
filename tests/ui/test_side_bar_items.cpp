#include <gtest/gtest.h>

#include <QAction>
#include <QMenu>
#include <QString>

#include "config/id_types.hpp"
#include "ui/side_bar/account_category.hpp"
#include "ui/side_bar/account_item.hpp"
#include "ui/side_bar/side_bar_item.hpp"

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

}   // namespace
