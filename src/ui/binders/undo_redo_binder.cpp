#include "undo_redo_binder.hpp"

#include <QStatusBar>

#include "ui/commands/undo_stack.hpp"
#include "ui/main_window.hpp"
#include "ui/menu_bar/menu_bar.hpp"

namespace ui
{
    UndoRedoBinder::UndoRedoBinder(
        MainWindow& mainWindow,
        MenuBar&    menuBar,
        UndoStack&  undoStack
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _menuBar(menuBar),
          _undoStack(undoStack)
    {
        connect(
            &_menuBar,
            &MenuBar::requestUndo,
            this,
            &UndoRedoBinder::_onUndoRequested
        );
        connect(
            &_menuBar,
            &MenuBar::requestRedo,
            this,
            &UndoRedoBinder::_onRedoRequested
        );

        connect(
            &_undoStack,
            &UndoStack::changed,
            this,
            &UndoRedoBinder::refresh
        );

        refresh();
    }

    void UndoRedoBinder::_onUndoRequested()
    {
        _undoStack.undo();
        _mainWindow.statusBar()->showMessage("Undo requested");
    }

    void UndoRedoBinder::_onRedoRequested()
    {
        _undoStack.redo();
        _mainWindow.statusBar()->showMessage("Redo requested");
    }

    void UndoRedoBinder::refresh()
    {
        const bool canUndo = _undoStack.canUndo();
        const bool canRedo = _undoStack.canRedo();

        _menuBar.setUndoEnabled(canUndo);
        _menuBar.setUndoText(
            canUndo ? QString::fromStdString("Undo " + _undoStack.undoLabel())
                    : "Undo"
        );

        _menuBar.setRedoEnabled(canRedo);
        _menuBar.setRedoText(
            canRedo ? QString::fromStdString("Redo " + _undoStack.redoLabel())
                    : "Redo"
        );
    }
}   // namespace ui
