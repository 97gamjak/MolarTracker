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
    class MenuBar;    // Forward declaration
    class Handlers;   // Forward declaration

    /**
     * @brief The main window of the application. This is the central widget
     * that contains the menu bar and the central widget.
     *
     */
    class MainWindow final : public QMainWindow
    {
        Q_OBJECT

       private:
        /// Reference to the application context
        app::AppContext& _appContext;
        /// Reference to the handlers container
        Handlers& _handlers;
        /// Pointer to the menu bar widget
        MenuBar* _menuBar = nullptr;

        /// File menu controller
        std::unique_ptr<FileMenuController> _fileMenuController;
        /// Edit menu controller
        std::unique_ptr<EditMenuController> _editMenuController;
        /// Debug menu controller
        std::unique_ptr<DebugMenuController> _debugMenuController;
        /// Settings menu controller
        std::unique_ptr<SettingsMenuController> _settingsMenuController;
        /// Help menu controller
        std::unique_ptr<HelpMenuController> _helpMenuController;

        /// Ensure profile controller
        std::unique_ptr<EnsureProfileController> _ensureProfileController;

        /// Undo stack for managing undoable commands in the application
        UndoStack _undoStack;

       public:
        explicit MainWindow(app::AppContext& app, Handlers& handlers);

        void start();
        void setWindowTitle(const bool& isDirty);

       private:
        void _buildUI();
        void _buildMenuBar();
        void _buildCentral();

        void _ensureProfileExists();
    };

}   // namespace ui

#endif   // __UI__MAIN_WINDOW_HPP__