#include "edit_menu.hpp"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace ui
{

    EditMenu::EditMenu(QMenuBar& menuBar) : QObject{&menuBar}
    {
        _editMenu = menuBar.addMenu("&Edit");

        _undoAction = _editMenu->addAction("&Undo");
        _undoAction->setShortcut(QKeySequence::Undo);
        _undoAction->setEnabled(false);
        connect(_undoAction, &QAction::triggered, this, &EditMenu::requestUndo);

        _redoAction = _editMenu->addAction("&Redo");
        _redoAction->setShortcut(QKeySequence::Redo);
        _redoAction->setEnabled(false);
        connect(_redoAction, &QAction::triggered, this, &EditMenu::requestRedo);
    }

}   // namespace ui