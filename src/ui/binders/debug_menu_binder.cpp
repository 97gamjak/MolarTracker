#include "debug_menu_binder.hpp"

#include <QStatusBar>

#include "ui/logging/debug_slots_dialog.hpp"
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
    }

    void DebugMenuBinder::_onRequestDebugSlots()
    {
        _dialog = new DebugSlotsDialog{_mainWindow};
        _dialog->setModal(false);

        _dialog->show();
        _dialog->raise();
        _dialog->activateWindow();

        _mainWindow.statusBar()->showMessage("Debug slots opened");
    }

}   // namespace ui
