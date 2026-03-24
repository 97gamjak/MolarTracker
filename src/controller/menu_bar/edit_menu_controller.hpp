#ifndef __CONTROLLER__MENU_BAR__EDIT_MENU_CONTROLLER_HPP__
#define __CONTROLLER__MENU_BAR__EDIT_MENU_CONTROLLER_HPP__

#include <QObject>

class QMainWindow;   // Forward declaration

namespace ui
{
    class EditMenu;    // Forward declaration
    class UndoStack;   // Forward declaration
}   // namespace ui

namespace controller
{
    /**
     * @brief Binds the edit menu actions to their respective functionalities.
     *
     */
    class EditMenuController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the edit menu
        ui::EditMenu& _editMenu;
        /// Reference to the undo stack
        ui::UndoStack& _undoStack;

       private slots:
        void _onUndoRequested();
        void _onRedoRequested();

       public:
        explicit EditMenuController(
            QMainWindow&   mainWindow,
            ui::EditMenu&  editMenu,
            ui::UndoStack& undoStack
        );

        void refresh();
    };

}   // namespace controller

#endif   // __CONTROLLER__MENU_BAR__EDIT_MENU_CONTROLLER_HPP__