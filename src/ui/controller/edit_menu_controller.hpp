#ifndef __UI__CONTROLLER__EDIT_MENU_CONTROLLER_HPP__
#define __UI__CONTROLLER__EDIT_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class EditMenu;
    class UndoStack;

    class EditMenuController : public QObject
    {
        Q_OBJECT

       private:
        QMainWindow& _mainWindow;
        EditMenu&    _editMenu;
        UndoStack&   _undoStack;

       private slots:
        void _onUndoRequested();
        void _onRedoRequested();

       public:
        explicit EditMenuController(
            QMainWindow& mainWindow,
            EditMenu&    editMenu,
            UndoStack&   undoStack
        );

        void refresh();
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__EDIT_MENU_CONTROLLER_HPP__