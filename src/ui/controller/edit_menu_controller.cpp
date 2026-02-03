#include "edit_menu_controller.hpp"

#include <QMainWindow>
#include <QStatusBar>

#include "ui/commands/undo_stack.hpp"
#include "ui/menu_bar/edit_menu.hpp"

namespace ui
{

    /**
     * @brief Construct a new Edit Menu Controller:: Edit Menu Controller object
     *
     * @param mainWindow The main window of the application
     * @param editMenu The edit menu containing undo/redo actions
     * @param undoStack The undo stack managing undo/redo operations
     */
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

    /**
     * @brief Slot called when an undo is requested from the UI
     *
     */
    void EditMenuController::_onUndoRequested()
    {
        _undoStack.undo();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Undo requested");
    }

    /**
     * @brief Slot called when a redo is requested from the UI
     *
     */
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