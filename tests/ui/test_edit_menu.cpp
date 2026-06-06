#include <gtest/gtest.h>

#include <QAction>
#include <QMenuBar>
#include <QString>

#include "ui/menu_bar/edit_menu.hpp"

namespace
{
    class EditMenuTest : public ::testing::Test
    {
       protected:
        QMenuBar     _menuBar;
        ui::EditMenu _editMenu{_menuBar};
    };

    TEST_F(EditMenuTest, UndoIsDisabledByDefault)
    {
        const auto* undoAction =
            _menuBar.findChild<QAction*>("", Qt::FindDirectChildrenOnly);
        // Locate undo via actions on the edit menu
        auto menus = _menuBar.findChildren<QMenu*>();
        ASSERT_FALSE(menus.isEmpty());

        bool undoFound   = false;
        bool undoEnabled = true;
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Undo", Qt::CaseInsensitive))
                {
                    undoFound   = true;
                    undoEnabled = action->isEnabled();
                }
            }
        }
        ASSERT_TRUE(undoFound);
        EXPECT_FALSE(undoEnabled);
    }

    TEST_F(EditMenuTest, RedoIsDisabledByDefault)
    {
        auto menus = _menuBar.findChildren<QMenu*>();
        ASSERT_FALSE(menus.isEmpty());

        bool redoFound   = false;
        bool redoEnabled = true;
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Redo", Qt::CaseInsensitive))
                {
                    redoFound   = true;
                    redoEnabled = action->isEnabled();
                }
            }
        }
        ASSERT_TRUE(redoFound);
        EXPECT_FALSE(redoEnabled);
    }

    TEST_F(EditMenuTest, SetUndoEnabledEnablesUndoAction)
    {
        _editMenu.setUndoEnabled(true);

        auto menus = _menuBar.findChildren<QMenu*>();
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Undo", Qt::CaseInsensitive))
                {
                    EXPECT_TRUE(action->isEnabled());
                    return;
                }
            }
        }
        FAIL() << "Undo action not found";
    }

    TEST_F(EditMenuTest, SetRedoEnabledEnablesRedoAction)
    {
        _editMenu.setRedoEnabled(true);

        auto menus = _menuBar.findChildren<QMenu*>();
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Redo", Qt::CaseInsensitive))
                {
                    EXPECT_TRUE(action->isEnabled());
                    return;
                }
            }
        }
        FAIL() << "Redo action not found";
    }

    TEST_F(EditMenuTest, SetUndoEnabledFalseDisablesIt)
    {
        _editMenu.setUndoEnabled(true);
        _editMenu.setUndoEnabled(false);

        auto menus = _menuBar.findChildren<QMenu*>();
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Undo", Qt::CaseInsensitive))
                {
                    EXPECT_FALSE(action->isEnabled());
                    return;
                }
            }
        }
        FAIL() << "Undo action not found";
    }

    TEST_F(EditMenuTest, SetUndoTextChangesActionText)
    {
        _editMenu.setUndoText("&Undo Create Profile");

        auto menus = _menuBar.findChildren<QMenu*>();
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Undo", Qt::CaseInsensitive))
                {
                    EXPECT_TRUE(action->text().contains(
                        "Create Profile",
                        Qt::CaseInsensitive
                    ));
                    return;
                }
            }
        }
        FAIL() << "Undo action not found after setText";
    }

    TEST_F(EditMenuTest, SetRedoTextChangesActionText)
    {
        _editMenu.setRedoText("&Redo Delete Profile");

        auto menus = _menuBar.findChildren<QMenu*>();
        for (auto* menu : menus)
        {
            for (auto* action : menu->actions())
            {
                if (action->text().contains("Redo", Qt::CaseInsensitive))
                {
                    EXPECT_TRUE(action->text().contains(
                        "Delete Profile",
                        Qt::CaseInsensitive
                    ));
                    return;
                }
            }
        }
        FAIL() << "Redo action not found after setText";
    }

}   // namespace
