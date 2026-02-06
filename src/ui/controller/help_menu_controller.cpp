#include "help_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>

#include "ui/widgets/menu_bar/help_menu.hpp"

namespace ui
{

    /**
     * @brief Construct a new Help Menu Controller:: Help Menu Controller object
     *
     * @param mainWindow
     * @param helpMenu
     */
    HelpMenuController::HelpMenuController(
        QMainWindow& mainWindow,
        HelpMenu&    helpMenu
    )
        : QObject{&mainWindow}, _mainWindow(mainWindow), _helpMenu(helpMenu)
    {
        connect(
            &_helpMenu,
            &HelpMenu::requestAbout,
            this,
            &HelpMenuController::_onAboutRequested
        );
    }

    /**
     * @brief Handle about request
     *
     */
    void HelpMenuController::_onAboutRequested()
    {
        // TODO(97gamjak): Update with real version and info
        // https://97gamjak.atlassian.net/browse/MOLTRACK-98
        QMessageBox::information(
            &_mainWindow,
            "About Molar Tracker",
            "Molar Tracker\nVersion 1.0.0\n© 2024 Molar Tracker Team"
        );
    }

}   // namespace ui