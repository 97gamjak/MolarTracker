#include "help_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>

#include "config/constants.hpp"
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

}   // namespace ui