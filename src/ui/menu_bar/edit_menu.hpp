#ifndef __UI__MENU_BAR__EDIT_MENU_HPP__
#define __UI__MENU_BAR__EDIT_MENU_HPP__

#include <QObject>

class QAction;    // Forward declaration
class QMenu;      // Forward declaration
class QMenuBar;   // Forward declaration

namespace ui
{
    class MenuBar;

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
    };

}   // namespace ui

#endif   // __UI__MENU_BAR__EDIT_MENU_HPP__