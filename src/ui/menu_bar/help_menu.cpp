#include "help_menu.hpp"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace ui
{

    HelpMenu::HelpMenu(QMenuBar& menuBar) : QObject{&menuBar}
    {
        _helpMenu = menuBar.addMenu("&Help");

        _aboutAction = _helpMenu->addAction("&About");
        connect(
            _aboutAction,
            &QAction::triggered,
            this,
            &HelpMenu::requestAbout
        );
    }

}   // namespace ui