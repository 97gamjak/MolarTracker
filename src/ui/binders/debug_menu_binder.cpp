#include "debug_menu_binder.hpp"

#include <QStatusBar>

#include "ui/logging/debug_slots_dialog.hpp"
#include "ui/logging/log_viewer_dialog.hpp"
#include "ui/main_window.hpp"
#include "ui/menu_bar/menu_bar.hpp"

namespace ui
{
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

    void DebugMenuBinder::_ensureDebugSlotsDialog()
    {
        if (_debugSlotDialog != nullptr)
            return;

        _debugSlotDialog = new DebugSlotsDialog{_mainWindow};
        _debugSlotDialog->setModal(false);
    }

    void DebugMenuBinder::_ensureLogViewerDialog()
    {
        if (_logViewerDialog != nullptr)
            return;

        _logViewerDialog = new LogViewerDialog{_mainWindow};
        _logViewerDialog->setModal(false);
    }

    void DebugMenuBinder::_onRequestDebugSlots()
    {
        _ensureDebugSlotsDialog();

        _debugSlotDialog->show();
        _debugSlotDialog->raise();
        _debugSlotDialog->activateWindow();

        _mainWindow.statusBar()->showMessage("Debug slots opened");
    }

    void DebugMenuBinder::_onRequestLogViewer()
    {
        _ensureLogViewerDialog();

        _logViewerDialog->show();
        _logViewerDialog->raise();
        _logViewerDialog->activateWindow();

        _mainWindow.statusBar()->showMessage("Log File opened");
    }

}   // namespace ui
