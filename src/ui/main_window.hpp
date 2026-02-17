#ifndef __UI__MAIN_WINDOW_HPP__
#define __UI__MAIN_WINDOW_HPP__

#include <QMainWindow>
#include <memory>

#include "commands/undo_stack.hpp"
#include "ui/controller/debug_menu_controller.hpp"
#include "ui/controller/edit_menu_controller.hpp"
#include "ui/controller/ensure_profile_controller.hpp"
#include "ui/controller/file_menu_controller.hpp"
#include "ui/controller/help_menu_controller.hpp"
#include "ui/controller/settings_menu_controller.hpp"

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace ui
{
    class MenuBar;       // Forward declaration
    class Controllers;   // Forward declaration

    class MainWindow final : public QMainWindow
    {
        Q_OBJECT

       private:
        app::AppContext& _appContext;
        Controllers&     _controllers;
        MenuBar*         _menuBar = nullptr;

        std::unique_ptr<FileMenuController>     _fileMenuController;
        std::unique_ptr<EditMenuController>     _editMenuController;
        std::unique_ptr<DebugMenuController>    _debugMenuController;
        std::unique_ptr<SettingsMenuController> _settingsMenuController;
        std::unique_ptr<HelpMenuController>     _helpMenuController;

        std::unique_ptr<EnsureProfileController> _ensureProfileController;

        UndoStack _undoStack;

       public:
        explicit MainWindow(
            app::AppContext& app,
            Controllers&     controllers,
            QWidget*         parent = nullptr
        );

       private:
        void _buildUI();
        void _buildMenuBar();
        void _buildCentral();

        void _ensureProfileExists();
    };

}   // namespace ui

#endif   // __UI__MAIN_WINDOW_HPP__