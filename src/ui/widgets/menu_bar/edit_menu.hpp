#ifndef __UI__MENU_BAR__EDIT_MENU_HPP__
#define __UI__MENU_BAR__EDIT_MENU_HPP__

#include <QObject>

class QAction;    // Forward declaration
class QMenu;      // Forward declaration
class QMenuBar;   // Forward declaration

namespace ui
{
    /**
     * @brief Edit menu in the menu bar
     *
     */
    class EditMenu : public QObject
    {
        Q_OBJECT

       signals:
        /// Signal emitted when the user requests to undo an action
        void requestUndo();
        /// Signal emitted when the user requests to redo an action
        void requestRedo();

       private:
        /// The edit menu widget
        QMenu* _editMenu = nullptr;

        /// The undo action in the edit menu
        QAction* _undoAction = nullptr;
        /// The redo action in the edit menu
        QAction* _redoAction = nullptr;

       public:
        explicit EditMenu(QMenuBar& menuBar);

        void setUndoEnabled(bool enabled);
        void setRedoEnabled(bool enabled);
        void setUndoText(const QString& text);
        void setRedoText(const QString& text);
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__EDIT_MENU_HPP__