#include "file_menu_controller.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QStatusBar>

#include "app/app_context.hpp"
#include "ui/menu_bar/file_menu.hpp"

namespace ui
{
    /**
     * @brief Construct a new FileMenuController:: File Menu Controller object
     *
     * @param mainWindow The main window of the application
     * @param menuBar The menu bar containing file menu actions
     * @param appContext The application context
     */
    FileMenuController::FileMenuController(
        QMainWindow&     mainWindow,
        FileMenu&        fileMenu,
        app::AppContext& appContext
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _fileMenu(fileMenu),
          _appContext(appContext)
    {
        connect(
            &_fileMenu,
            &FileMenu::requestSave,
            this,
            &FileMenuController::_onRequestSave
        );

        connect(
            &_fileMenu,
            &FileMenu::requestQuit,
            this,
            &FileMenuController::_onRequestQuit
        );
    }

    /**
     * @brief Slot called when a save is requested from the UI
     *
     */
    void FileMenuController::_onRequestSave()
    {
        _appContext.getStore().commit();
        _appContext.getSettings().save();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Save requested");
    }

    /**
     * @brief Slot called when a quit is requested from the UI
     *
     */
    void FileMenuController::_onRequestQuit() { _mainWindow.close(); }

}   // namespace ui