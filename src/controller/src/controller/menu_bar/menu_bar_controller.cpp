#include "menu_bar_controller.hpp"

#include "debug_menu_controller.hpp"
#include "edit_menu_controller.hpp"
#include "file_menu_controller.hpp"
#include "help_menu_controller.hpp"
#include "settings_menu_controller.hpp"
#include "store/store_container.hpp"
#include "ui/main_window.hpp"
#include "ui/menu_bar/menu_bar.hpp"

namespace controller
{
    /**
     * @brief Construct a new Menu Bar Controller:: Menu Bar Controller object
     *
     * @param mainWindow The main window of the application
     * @param menuBar The menu bar of the application
     * @param storeContainer The store container for managing application state
     * @param undoStack The undo stack for managing undoable commands in the
     * application
     * @param settings The settings for the application
     */
    MenuBarController::MenuBarController(
        ui::MainWindow*        mainWindow,
        ui::MenuBar&           menuBar,
        store::StoreContainer& storeContainer,
        cmd::UndoStack&        undoStack,
        settings::Settings&    settings
    )
    {
        _makeControllers(
            mainWindow,
            menuBar,
            storeContainer,
            undoStack,
            settings
        );
    }

    MenuBarController::~MenuBarController() = default;

    /**
     * @brief Create the controllers for each menu in the menu bar
     *
     * @param mainWindow The main window of the application
     * @param menuBar The menu bar of the application
     * @param storeContainer The store container for managing application state
     * @param undoStack The undo stack for managing undoable commands in the
     * application
     * @param settings The settings for the application
     */
    void MenuBarController::_makeControllers(
        ui::MainWindow*        mainWindow,
        ui::MenuBar&           menuBar,
        store::StoreContainer& storeContainer,
        cmd::UndoStack&        undoStack,
        settings::Settings&    settings
    )
    {
        _fileMenuController = std::make_unique<FileMenuController>(
            *mainWindow,
            menuBar.getFileMenu(),
            storeContainer,
            settings
        );

        _editMenuController = std::make_unique<EditMenuController>(
            *mainWindow,
            menuBar.getEditMenu(),
            undoStack
        );

        _debugMenuController = std::make_unique<DebugMenuController>(
            *mainWindow,
            menuBar.getDebugMenu(),
            undoStack,
            settings
        );

        _settingsMenuController = std::make_unique<SettingsMenuController>(
            *mainWindow,
            menuBar.getSettingsMenu(),
            settings
        );

        _helpMenuController = std::make_unique<HelpMenuController>(
            *mainWindow,
            menuBar.getHelpMenu()
        );
    }

}   // namespace controller