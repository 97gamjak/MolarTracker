#ifndef __CONTROLLER__MENU_BAR__MENU_BAR_CONTROLLER_HPP__
#define __CONTROLLER__MENU_BAR__MENU_BAR_CONTROLLER_HPP__

#include <memory>

#include "debug_menu_controller.hpp"
#include "edit_menu_controller.hpp"
#include "file_menu_controller.hpp"
#include "help_menu_controller.hpp"
#include "settings_menu_controller.hpp"

namespace ui
{
    class MenuBar;      // Forward declaration
    class MainWindow;   // Forward declaration
}   // namespace ui

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace controller
{
    /**
     * @brief Controller for the menu bar, this is responsible for creating and
     * managing the controllers for each menu in the menu bar
     *
     */
    class MenuBarController
    {
       private:
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

       public:
        explicit MenuBarController(
            ui::MainWindow*  mainWindow,
            ui::MenuBar&     menuBar,
            app::AppContext& appContext,
            cmd::UndoStack&  undoStack
        );

        void _makeControllers(
            ui::MainWindow*  mainWindow,
            ui::MenuBar&     menuBar,
            app::AppContext& appContext,
            cmd::UndoStack&  undoStack
        );
    };

}   // namespace controller

#endif   // __CONTROLLER__MENU_BAR__MENU_BAR_CONTROLLER_HPP__