#include "edit_menu.hpp"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace ui
{

    /**
     * @brief Construct a new Edit Menu:: Edit Menu object
     *
     * @param menuBar
     */
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

    /**
     * @brief Enable or disable the Undo action
     *
     * @param enabled
     */
    void EditMenu::setUndoEnabled(bool enabled)
    {
        _undoAction->setEnabled(enabled);
    }

    /**
     * @brief Enable or disable the Redo action
     *
     * @param enabled
     */
    void EditMenu::setRedoEnabled(bool enabled)
    {
        _redoAction->setEnabled(enabled);
    }

    /**
     * @brief Set the text for the Undo action
     *
     * @param text
     */
    void EditMenu::setUndoText(const QString& text)
    {
        _undoAction->setText(text);
    }

    /**
     * @brief Set the text for the Redo action
     *
     * @param text
     */
    void EditMenu::setRedoText(const QString& text)
    {
        _redoAction->setText(text);
    }

}   // namespace ui