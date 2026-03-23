#include "menu_bar_controller.hpp"

#include "ui/main_window.hpp"
#include "ui/widgets/menu_bar/menu_bar.hpp"

namespace ui
{
    /**
     * @brief Construct a new Menu Bar Controller:: Menu Bar Controller object
     *
     * @param mainWindow The main window of the application
     * @param menuBar The menu bar of the application
     * @param appContext The application context
     * @param undoStack The undo stack for managing undoable commands in the
     * application
     */
    MenuBarController::MenuBarController(
        MainWindow*      mainWindow,
        MenuBar&         menuBar,
        app::AppContext& appContext,
        UndoStack&       undoStack
    )
    {
        _makeControllers(mainWindow, menuBar, appContext, undoStack);
    }

    /**
     * @brief Create the controllers for each menu in the menu bar
     *
     * @param mainWindow The main window of the application
     * @param menuBar The menu bar of the application
     * @param appContext The application context
     * @param undoStack The undo stack for managing undoable commands in the
     * application
     */
    void MenuBarController::_makeControllers(
        MainWindow*      mainWindow,
        MenuBar&         menuBar,
        app::AppContext& appContext,
        UndoStack&       undoStack
    )
    {
        _fileMenuController = std::make_unique<FileMenuController>(
            *mainWindow,
            menuBar.getFileMenu(),
            appContext
        );

        _editMenuController = std::make_unique<EditMenuController>(
            *mainWindow,
            menuBar.getEditMenu(),
            undoStack
        );

        _debugMenuController = std::make_unique<DebugMenuController>(
            *mainWindow,
            menuBar.getDebugMenu(),
            appContext,
            undoStack
        );

        _settingsMenuController = std::make_unique<SettingsMenuController>(
            *mainWindow,
            menuBar.getSettingsMenu()
        );

        _helpMenuController = std::make_unique<HelpMenuController>(
            *mainWindow,
            menuBar.getHelpMenu()
        );
    }

}   // namespace ui