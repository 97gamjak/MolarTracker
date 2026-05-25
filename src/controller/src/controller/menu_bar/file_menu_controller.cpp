#include "file_menu_controller.hpp"

#include <QMenuBar>
#include <QObject>
#include <QStatusBar>

#include "settings/settings.hpp"
#include "store/store_container.hpp"
#include "ui/main_window.hpp"
#include "ui/menu_bar/file_menu.hpp"

namespace controller
{
    /**
     * @brief Construct a new FileMenuController:: File Menu Controller object
     *
     * @param mainWindow The main window of the application
     * @param fileMenu The file menu containing save/quit actions
     * @param storeContainer The store container for managing application state
     * @param settings The application settings
     */
    FileMenuController::FileMenuController(
        ui::MainWindow&        mainWindow,
        ui::FileMenu&          fileMenu,
        store::StoreContainer& storeContainer,
        settings::Settings&    settings
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _fileMenu(fileMenu),
          _storeContainer(storeContainer),
          _settings(settings)
    {
        connect(
            &_fileMenu,
            &ui::FileMenu::requestSave,
            this,
            &FileMenuController::_onRequestSave
        );

        connect(
            &_fileMenu,
            &ui::FileMenu::requestQuit,
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
        _settings.save();
        _storeContainer.commit();
        _storeContainer.clearPotentiallyDirty();

        _mainWindow.setWindowTitle(false);

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar != nullptr)
            statusBar->showMessage("Save requested");
    }

    /**
     * @brief Slot called when a quit is requested from the UI
     *
     */
    void FileMenuController::_onRequestQuit() { _mainWindow.close(); }

}   // namespace controller