#include "debug_menu_binder.hpp"

#include <QStatusBar>

#include "ui/logging/debug_slots_dialog.hpp"
#include "ui/logging/log_viewer_dialog.hpp"
#include "ui/main_window.hpp"
#include "ui/menu_bar/menu_bar.hpp"

namespace ui
{
    /**
     * @brief Construct a new DebugMenuBinder:: Debug Menu Binder object
     *
     * @param mainWindow The main window of the application
     * @param menuBar The menu bar containing debug menu actions
     */
    DebugMenuBinder::DebugMenuBinder(MainWindow& mainWindow, MenuBar& menuBar)
        : QObject(&mainWindow), _mainWindow(mainWindow), _menuBar(menuBar)
    {
        connect(
            &_menuBar,
            &MenuBar::requestDebugSlots,
            this,
            &DebugMenuBinder::_onRequestDebugSlots
        );

        connect(
            &_menuBar,
            &MenuBar::requestLogViewer,
            this,
            &DebugMenuBinder::_onRequestLogViewer
        );
    }

    /**
     * @brief Ensures that the Debug Slots dialog is created
     *
     */
    void DebugMenuBinder::_ensureDebugSlotsDialog()
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
    void DebugMenuBinder::_ensureLogViewerDialog()
    {
        if (_logViewerDialog != nullptr)
            return;

        _logViewerDialog = new LogViewerDialog{_mainWindow};
        _logViewerDialog->setModal(false);
    }

    /**
     * @brief Slot called when the Debug Slots dialog is requested from the UI
     *
     */
    void DebugMenuBinder::_onRequestDebugSlots()
    {
        _ensureDebugSlotsDialog();

        _debugSlotsDialog->show();
        _debugSlotsDialog->raise();
        _debugSlotsDialog->activateWindow();

        _mainWindow.statusBar()->showMessage("Debug slots opened");
    }

    /**
     * @brief Slot called when the Log Viewer dialog is requested from the UI
     *
     */
    void DebugMenuBinder::_onRequestLogViewer()
    {
        _ensureLogViewerDialog();

        _logViewerDialog->show();
        _logViewerDialog->raise();
        _logViewerDialog->activateWindow();

        _mainWindow.statusBar()->showMessage("Log File opened");
    }

}   // namespace ui
