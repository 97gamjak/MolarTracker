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
        void requestUndo();
        void requestRedo();

       private:
        QMenu* _editMenu = nullptr;

        QAction* _undoAction = nullptr;
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