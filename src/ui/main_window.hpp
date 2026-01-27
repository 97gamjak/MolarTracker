#ifndef __UI__MAIN_WINDOW_HPP__
#define __UI__MAIN_WINDOW_HPP__

#include <QMainWindow>

#include "undo_redo/undo_stack.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace ui
{
    class MenuBar;           // Forward declaration
    class UndoRedoBinder;    // Forward declaration
    class DebugMenuBinder;   // Forward declaration

    class MainWindow final : public QMainWindow
    {
        Q_OBJECT

       private:
        app::AppContext& _appContext;
        MenuBar*         _menuBar         = nullptr;
        UndoRedoBinder*  _undoRedoBinder  = nullptr;
        DebugMenuBinder* _debugMenuBinder = nullptr;

        UndoStack _undoStack;
        bool      _dummyFlag = false;

       public:
        explicit MainWindow(app::AppContext& app, QWidget* parent = nullptr);

       private:
        void _buildUI();
        void _buildMenuBar();
        void _buildCentral();

        void _ensureProfileExists();

        void _onSaveRequested();
        void _onPreferencesRequested();
        void _onAboutRequested();
    };

}   // namespace ui

#endif   // __UI__MAIN_WINDOW_HPP__