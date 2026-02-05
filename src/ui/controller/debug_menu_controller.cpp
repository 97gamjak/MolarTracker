#include "debug_menu_controller.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QStatusBar>

#include "app/app_context.hpp"
#include "ui/logging/debug_slots_dialog.hpp"
#include "ui/logging/log_viewer_dialog.hpp"
#include "ui/menu_bar/debug_menu.hpp"

namespace ui
{
    /**
     * @brief Construct a new Debug Menu Controller:: Debug Menu Controller
     * object
     *
     * @param mainWindow
     * @param debugMenu
     * @param appContext
     */
    DebugMenuController::DebugMenuController(
        QMainWindow&     mainWindow,
        DebugMenu&       debugMenu,
        app::AppContext& appContext
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _debugMenu(debugMenu),
          _appContext(appContext)
    {
        connect(
            &debugMenu,
            &DebugMenu::requestDebugSlots,
            this,
            &DebugMenuController::_onRequestDebugSlots
        );

        connect(
            &debugMenu,
            &DebugMenu::requestLogViewer,
            this,
            &DebugMenuController::_onRequestLogViewer
        );
    }

    /**
     * @brief Handle log viewer request
     *
     */
    void DebugMenuController::_onRequestDebugSlots()
    {
        _ensureDebugSlotsDialog();

        _debugSlotsDialog->show();
        _debugSlotsDialog->raise();
        _debugSlotsDialog->activateWindow();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Debug slots opened");
    }

    /**
     * @brief Handle debug slots request
     *
     */
    void DebugMenuController::_onRequestLogViewer()
    {
        _ensureLogViewerDialog();

        _logViewerDialog->show();
        _logViewerDialog->raise();
        _logViewerDialog->activateWindow();

        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Log File opened");
    }

    /**
     * @brief Ensures that the Debug Slots dialog is created
     *
     */
    void DebugMenuController::_ensureDebugSlotsDialog()
    {
        if (_debugSlotsDialog != nullptr)
            return;

        _debugSlotsDialog = new DebugSlotsDialog{_mainWindow};
        _debugSlotsDialog->setModal(false);
    }

    /**
     * @brief Ensures that the Log Viewer dialog is created
     *
     */
    void DebugMenuController::_ensureLogViewerDialog()
    {
        if (_logViewerDialog)
            return;

        _logViewerDialog = new LogViewerDialog{&_mainWindow};
        _logViewerDialog->setModal(false);
    }

}   // namespace ui