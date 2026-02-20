#include "edit_menu_controller.hpp"

#include <QMainWindow>
#include <QStatusBar>

#include "ui/commands/undo_stack.hpp"
#include "ui/widgets/menu_bar/edit_menu.hpp"

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
        connect(
            &_undoStack,
            &UndoStack::changed,
            this,
            &EditMenuController::refresh
        );

        refresh();
    }

    /**
     * @brief Slot called when an undo is requested from the UI
     *
     */
    void EditMenuController::_onUndoRequested()
    {
        std::string msg = _undoStack.getUndoLabel();

        const auto& result = _undoStack.undo();

        if (!result)
            msg = result.error()->getMessage();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage(QString::fromStdString(msg));
    }

    /**
     * @brief Slot called when a redo is requested from the UI
     *
     */
    void EditMenuController::_onRedoRequested()
    {
        std::string msg = _undoStack.getRedoLabel();

        const auto& result = _undoStack.redo();

        if (!result)
            msg = result.error()->getMessage();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage(QString::fromStdString(msg));
    }

    /**
     * @brief Refresh the state of the edit menu actions based on the undo stack
     * state, this should be called whenever the undo stack changes to update
     * the enabled state and text of the undo/redo actions in the edit menu.
     *
     */
    void EditMenuController::refresh()
    {
        const bool canUndo = _undoStack.canUndo();
        const bool canRedo = _undoStack.canRedo();

        _editMenu.setUndoEnabled(canUndo);
        _editMenu.setUndoText(
            canUndo
                ? QString::fromStdString("Undo " + _undoStack.getUndoLabel())
                : "Undo"
        );

        _editMenu.setRedoEnabled(canRedo);
        _editMenu.setRedoText(
            canRedo
                ? QString::fromStdString("Redo " + _undoStack.getRedoLabel())
                : "Redo"
        );
    }

}   // namespace ui