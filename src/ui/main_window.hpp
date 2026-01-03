#ifndef __UI__MAIN_WINDOW_HPP__
#define __UI__MAIN_WINDOW_HPP__

#include <QMainWindow>

#include "undo_redo/undo_stack.hpp"

class AppContext;   // Forward declaration

namespace ui
{
    class TopMenuBar;   // Forward declaration

    class MainWindow final : public QMainWindow
    {
        Q_OBJECT

       private:
        AppContext& _appContext;
        TopMenuBar* _topMenuBar = nullptr;

        UndoStack _undoStack;
        bool      _dummyFlag = false;

       private:
        void _buildUI();
        void _buildCentral();

        void _refreshUndoRedoActions();

        void _ensureDefaultProfile();

       public:
        explicit MainWindow(AppContext& appContext, QWidget* parent = nullptr);
    };

}   // namespace ui

#endif   // __UI__MAIN_WINDOW_HPP__