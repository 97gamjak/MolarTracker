#include "ui/menu_bar/help_menu.hpp"

#include <QAction>
#include <QMenu>
#include <QMenuBar>

namespace ui
{

    /**
     * @brief Construct a new Help Menu:: Help Menu object
     *
     * @param menuBar
     */
    HelpMenu::HelpMenu(QMenuBar& menuBar) : QObject{&menuBar}
    {
        _helpMenu = menuBar.addMenu("&Help");

        _helpPageAction = _helpMenu->addAction("&Help");
        connect(
            _helpPageAction,
            &QAction::triggered,
            this,
            &HelpMenu::requestHelpPage
        );

        _migrationHistoryAction = _helpMenu->addAction("&Migration History");
        connect(
            _migrationHistoryAction,
            &QAction::triggered,
            this,
            &HelpMenu::requestMigrationHistory
        );

        _helpMenu->addSeparator();

        _aboutAction = _helpMenu->addAction("&About");
        connect(
            _aboutAction,
            &QAction::triggered,
            this,
            &HelpMenu::requestAbout
        );
    }

}   // namespace ui