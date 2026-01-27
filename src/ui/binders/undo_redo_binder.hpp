#ifndef __UI__BINDERS__UNDO_REDO_BINDER_HPP__
#define __UI__BINDERS__UNDO_REDO_BINDER_HPP__

#include <QObject>

namespace ui
{
    class MainWindow;   // Forward declaration
    class TopMenuBar;   // Forward declaration
    class UndoStack;    // Forward declaration

    class UndoRedoBinder final : public QObject
    {
        Q_OBJECT

       private:
        MainWindow& _mainWindow;
        TopMenuBar& _menuBar;
        UndoStack&  _undoStack;

       private slots:
        void _onUndoRequested();
        void _onRedoRequested();

       public:
        UndoRedoBinder(
            MainWindow& mainWindow,
            TopMenuBar& menuBar,
            UndoStack&  undoStack
        );

        void refresh();
    };

}   // namespace ui

#endif   // __UI__BINDERS__UNDO_REDO_BINDER_HPP__