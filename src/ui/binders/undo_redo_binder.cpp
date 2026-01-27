#include "ui/binders/undo_redo_binder.hpp"

#include <QStatusBar>

#include "ui/main_window.hpp"
#include "ui/top_menu_bar.hpp"
#include "ui/undo_redo/undo_stack.hpp"

namespace ui
{
    UndoRedoBinder::UndoRedoBinder(
        MainWindow& mainWindow,
        TopMenuBar& menuBar,
        UndoStack&  undoStack
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _menuBar(menuBar),
          _undoStack(undoStack)
    {
        connect(
            &_menuBar,
            &TopMenuBar::requestUndo,
            this,
            &UndoRedoBinder::_onUndoRequested
        );
        connect(
            &_menuBar,
            &TopMenuBar::requestRedo,
            this,
            &UndoRedoBinder::_onRedoRequested
        );

        // If you add this signal later, you get automatic refresh everywhere:
        // connect(&_undoStack, &UndoStack::changed, this,
        // &UndoRedoBinder::refresh);

        refresh();
    }

    void UndoRedoBinder::_onUndoRequested()
    {
        _undoStack.undo();
        _mainWindow.statusBar()->showMessage("Undo requested");
        refresh();
    }

    void UndoRedoBinder::_onRedoRequested()
    {
        _undoStack.redo();
        _mainWindow.statusBar()->showMessage("Redo requested");
        refresh();
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
