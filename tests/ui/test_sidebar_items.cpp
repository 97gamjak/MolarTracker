// test_sidebar_items.cpp
//
// GoogleTest-based tests for ui::SideBarItem, ui::AccountItem,
// and ui::AccountCategory.
//
// Coverage:
//  - SideBarItem::getType() returns the type passed to constructor
//  - SideBarItem::setName() updates displayed text
//  - AccountItem::getId() returns the ID passed to constructor
//  - AccountItem::getOpenAction()/getDeleteAction() return nullptr before menu
//  - AccountItem::populateContextMenu() adds Open and Delete actions
//  - AccountCategory constructed with name "Accounts"
//  - AccountCategory::addAccount() appends an AccountItem child
//  - AccountCategory::clearAccounts() removes all children
//  - AccountCategory::getCreateAction() returns nullptr before menu
//  - AccountCategory::populateContextMenu() adds Create Account action

#include <gtest/gtest.h>

#include <QMenu>
#include <QString>

#include "config/id_types.hpp"
#include "ui/side_bar/account_category.hpp"
#include "ui/side_bar/account_item.hpp"
#include "ui/side_bar/side_bar_item.hpp"

namespace
{

    // --------------- AccountItem ------------------------------------------

    class AccountItemTest : public ::testing::Test
    {
       protected:
        const AccountId    _id{AccountId::from(42)};
        ui::AccountItem    _item{_id, "My Account"};
    };

    TEST_F(AccountItemTest, GetTypeReturnsAccountsItem)
    {
        EXPECT_EQ(_item.getType(), ui::SideBarItemType::AccountsItem);
    }

    TEST_F(AccountItemTest, GetIdReturnsConstructorId)
    {
        EXPECT_EQ(_item.getId(), _id);
    }

    TEST_F(AccountItemTest, InitialNameMatchesText)
    {
        EXPECT_EQ(_item.text(), "My Account");
    }

    TEST_F(AccountItemTest, SetNameUpdatesText)
    {
        _item.setName("Renamed Account");
        EXPECT_EQ(_item.text(), "Renamed Account");
    }

    TEST_F(AccountItemTest, OpenActionNullBeforeMenuPopulation)
    {
        EXPECT_EQ(_item.getOpenAction(), nullptr);
    }

    TEST_F(AccountItemTest, DeleteActionNullBeforeMenuPopulation)
    {
        EXPECT_EQ(_item.getDeleteAction(), nullptr);
    }

    TEST_F(AccountItemTest, PopulateContextMenuAddsOpenAndDeleteActions)
    {
        QMenu menu;
        _item.populateContextMenu(menu);

        ASSERT_NE(_item.getOpenAction(), nullptr);
        ASSERT_NE(_item.getDeleteAction(), nullptr);

        EXPECT_EQ(_item.getOpenAction()->text(), "Open");
        EXPECT_EQ(_item.getDeleteAction()->text(), "Delete");
    }

    TEST_F(AccountItemTest, ItemIsNotEditable)
    {
        EXPECT_FALSE(_item.isEditable());
    }

    // --------------- AccountCategory -------------------------------------

    class AccountCategoryTest : public ::testing::Test
    {
       protected:
        ui::AccountCategory _cat;
    };

    TEST_F(AccountCategoryTest, GetTypeReturnsAccountCategory)
    {
        EXPECT_EQ(_cat.getType(), ui::SideBarItemType::AccountCategory);
    }

    TEST_F(AccountCategoryTest, InitialNameIsAccounts)
    {
        EXPECT_EQ(_cat.text(), "Accounts");
    }

    TEST_F(AccountCategoryTest, NoChildrenInitially)
    {
        EXPECT_EQ(_cat.rowCount(), 0);
    }

    TEST_F(AccountCategoryTest, AddAccountAppendsChild)
    {
        _cat.addAccount(AccountId::from(1), "Savings");
        EXPECT_EQ(_cat.rowCount(), 1);
    }

    TEST_F(AccountCategoryTest, AddMultipleAccountsIncreasesChildCount)
    {
        _cat.addAccount(AccountId::from(1), "Savings");
        _cat.addAccount(AccountId::from(2), "Checking");
        _cat.addAccount(AccountId::from(3), "Brokerage");
        EXPECT_EQ(_cat.rowCount(), 3);
    }

    TEST_F(AccountCategoryTest, ClearAccountsRemovesAllChildren)
    {
        _cat.addAccount(AccountId::from(1), "A");
        _cat.addAccount(AccountId::from(2), "B");
        ASSERT_EQ(_cat.rowCount(), 2);
        _cat.clearAccounts();
        EXPECT_EQ(_cat.rowCount(), 0);
    }

    TEST_F(AccountCategoryTest, CreateActionNullBeforeMenuPopulation)
    {
        EXPECT_EQ(_cat.getCreateAction(), nullptr);
    }

    TEST_F(AccountCategoryTest, PopulateContextMenuAddsCreateAction)
    {
        QMenu menu;
        _cat.populateContextMenu(menu);

        ASSERT_NE(_cat.getCreateAction(), nullptr);
        EXPECT_EQ(_cat.getCreateAction()->text(), "Create Account");
    }

    TEST_F(AccountCategoryTest, CategoryIsNotEditable)
    {
        EXPECT_FALSE(_cat.isEditable());
    }

    TEST_F(AccountCategoryTest, ClearAfterClearIsNoop)
    {
        _cat.clearAccounts();
        EXPECT_EQ(_cat.rowCount(), 0);
    }

    TEST_F(AccountCategoryTest, AddedChildHasCorrectName)
    {
        _cat.addAccount(AccountId::from(5), "Investment");
        const auto* child =
            dynamic_cast<ui::AccountItem*>(_cat.child(0));
        ASSERT_NE(child, nullptr);
        EXPECT_EQ(child->text(), "Investment");
    }

    TEST_F(AccountCategoryTest, AddedChildHasCorrectId)
    {
        _cat.addAccount(AccountId::from(7), "Trading");
        const auto* child =
            dynamic_cast<ui::AccountItem*>(_cat.child(0));
        ASSERT_NE(child, nullptr);
        EXPECT_EQ(child->getId(), AccountId::from(7));
    }

}   // namespace
