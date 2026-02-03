#include "debug_menu.hpp"

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

namespace ui
{

    DebugMenu::DebugMenu(QMenuBar& menuBar) : QObject(&menuBar)
    {
        _debugMenu = menuBar.addMenu("&Debug");

        _debugSlotsAction = _debugMenu->addAction("Debug Slots");
        connect(
            _debugSlotsAction,
            &QAction::triggered,
            this,
            &DebugMenu::requestDebugSlots
        );

        _logViewerAction = _debugMenu->addAction("View Log File");
        connect(
            _logViewerAction,
            &QAction::triggered,
            this,
            &DebugMenu::requestLogViewer
        );
    }

}   // namespace ui