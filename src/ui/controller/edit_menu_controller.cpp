#include "edit_menu_controller.hpp"

#include <QMainWindow>
#include <QStatusBar>

#include "ui/commands/undo_stack.hpp"
#include "ui/menu_bar/edit_menu.hpp"

namespace ui
{

    EditMenuController::EditMenuController(
        QMainWindow& mainWindow,
        EditMenu&    editMenu,
        UndoStack&   undoStack
    )
        : QObject{&mainWindow},
          _mainWindow(mainWindow),
          _editMenu(editMenu),
          _undoStack(undoStack)
    {
        connect(
            &_editMenu,
            &EditMenu::requestUndo,
            this,
            &EditMenuController::_onUndoRequested
        );
        connect(
            &_editMenu,
            &EditMenu::requestRedo,
            this,
            &EditMenuController::_onRedoRequested
        );
    }

    void EditMenuController::_onUndoRequested()
    {
        _undoStack.undo();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Undo requested");
    }

    void EditMenuController::_onRedoRequested()
    {
        _undoStack.redo();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Redo requested");
    }

    void EditMenuController::refresh()
    {
        const bool canUndo = _undoStack.canUndo();
        const bool canRedo = _undoStack.canRedo();

        _editMenu.setUndoEnabled(canUndo);
        _editMenu.setUndoText(
            canUndo ? QString::fromStdString("Undo " + _undoStack.undoLabel())
                    : "Undo"
        );

        _editMenu.setRedoEnabled(canRedo);
        _editMenu.setRedoText(
            canRedo ? QString::fromStdString("Redo " + _undoStack.redoLabel())
                    : "Redo"
        );
    }

}   // namespace ui