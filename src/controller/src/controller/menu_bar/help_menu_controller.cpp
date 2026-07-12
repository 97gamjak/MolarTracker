#include "help_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QString>

#include "config/constants.hpp"
#include "ui/help/help_dialog.hpp"
#include "ui/menu_bar/help_menu.hpp"

namespace controller
{

    /**
     * @brief Construct a new Help Menu Controller:: Help Menu Controller object
     *
     * @param mainWindow
     * @param helpMenu
     */
    HelpMenuController::HelpMenuController(
        QMainWindow&  mainWindow,
        ui::HelpMenu& helpMenu
    )
        : QObject{&mainWindow}, _mainWindow(mainWindow), _helpMenu(helpMenu)
    {
        connect(
            &_helpMenu,
            &ui::HelpMenu::requestHelpPage,
            this,
            &HelpMenuController::_onHelpPageRequested
        );
        connect(
            &_helpMenu,
            &ui::HelpMenu::requestAbout,
            this,
            &HelpMenuController::_onAboutRequested
        );
    }

    /**
     * @brief Handle help page request
     *
     */
    void HelpMenuController::_onHelpPageRequested()
    {
        ui::HelpDialog dlg(&_mainWindow);
        dlg.exec();
    }

    /**
     * @brief Handle about request
     *
     */
    void HelpMenuController::_onAboutRequested()
    {
        const auto version = QString::fromStdString(Constants::getVersion());
        const auto githubRepoUrl =
            QString::fromStdString(Constants::getGithubRepoUrl());

        const auto aboutText =
            QString(
                "Molar Tracker %1\n\n"
                "A simple application to track your molar intake and related "
                "information.\n\n"
                "GitHub: %2\n"
            )
                .arg(version)
                .arg(githubRepoUrl);

        QMessageBox::information(
            &_mainWindow,
            "About Molar Tracker",
            aboutText
        );
    }

}   // namespace controller