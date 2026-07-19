#ifndef __CONTROLLER__SRC__CONTROLLER__MENU_BAR__MENU_BAR_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__MENU_BAR__MENU_BAR_CONTROLLER_HPP__

#include <memory>

namespace ui
{
    class MenuBar;      // Forward declaration
    class MainWindow;   // Forward declaration
}   // namespace ui

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace store
{
    class StoreContainer;   // Forward declaration
}   // namespace store

namespace settings
{
    class Settings;   // Forward declaration
}   // namespace settings

namespace controller
{
    class FileMenuController;
    class EditMenuController;
    class DebugMenuController;
    class SettingsMenuController;
    class HelpMenuController;

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
            ui::MainWindow*        mainWindow,
            ui::MenuBar&           menuBar,
            store::StoreContainer& storeContainer,
            cmd::UndoStack&        undoStack,
            settings::Settings&    settings
        );
        ~MenuBarController();

        void _makeControllers(
            ui::MainWindow*        mainWindow,
            ui::MenuBar&           menuBar,
            store::StoreContainer& storeContainer,
            cmd::UndoStack&        undoStack,
            settings::Settings&    settings
        );

        [[nodiscard]] SettingsMenuController& getSettingsMenuController();
    };

}   // namespace controller

#endif   // __CONTROLLER__SRC__CONTROLLER__MENU_BAR__MENU_BAR_CONTROLLER_HPP__