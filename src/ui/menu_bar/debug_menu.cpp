#include "debug_menu.hpp"

#include <QMainWindow>
#include <QMenu>

namespace ui
{

    DebugMenu::DebugMenu(QMainWindow& mainWindow)
        : QObject{&mainWindow}, _mainWindow{mainWindow}
    {
    }

    void DebugMenu::build(QMenu* parentMenu)
    {
        _debugSlotsAction = parentMenu->addAction("Debug Slots");
        connect(
            _debugSlotsAction,
            &QAction::triggered,
            this,
            &DebugMenu::requestDebugSlots
        );

        _logViewerAction = parentMenu->addAction("View Log File");
        connect(
            _logViewerAction,
            &QAction::triggered,
            this,
            &DebugMenu::requestLogViewer
        );
    }

}   // namespace ui