#include "help_menu_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QString>

#include "config/constants/github_constants.hpp"
#include "service/i_migration_log_service.hpp"
#include "ui/help/help_dialog.hpp"
#include "ui/menu_bar/help_menu.hpp"
#include "ui/migration/migration_history_dialog.hpp"

namespace controller
{

    /**
     * @brief Construct a new Help Menu Controller:: Help Menu Controller object
     *
     * @param mainWindow
     * @param helpMenu
     * @param migrationLogService
     */
    HelpMenuController::HelpMenuController(
        QMainWindow&  mainWindow,
        ui::HelpMenu& helpMenu,
        const std::shared_ptr<service::IMigrationLogService>&
            migrationLogService
    )
        : QObject{&mainWindow},
          _mainWindow(mainWindow),
          _helpMenu(helpMenu),
          _migrationLogService(migrationLogService)
    {
        connect(
            &_helpMenu,
            &ui::HelpMenu::requestHelpPage,
            this,
            &HelpMenuController::_onHelpPageRequested
        );
        connect(
            &_helpMenu,
            &ui::HelpMenu::requestMigrationHistory,
            this,
            &HelpMenuController::_onMigrationHistoryRequested
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
     * @brief Handle migration history request
     *
     */
    void HelpMenuController::_onMigrationHistoryRequested()
    {
        std::vector<ui::MigrationHistoryEntry> entries;
        for (const auto& entry : _migrationLogService->getAll())
        {
            entries.push_back(
                ui::MigrationHistoryEntry{
                    .fromVersion    = entry.fromVersion,
                    .toVersion      = entry.toVersion,
                    .releaseVersion = entry.releaseVersion,
                    .appliedAt      = entry.appliedAt,
                }
            );
        }
        // (entry here is a service::MigrationLogEntry, mapped to the UI's
        // own MigrationHistoryEntry — each layer keeps its own plain DTO
        // rather than leaking repo:: types up through service's public
        // interface)

        ui::MigrationHistoryDialog dlg(
            std::move(entries),
            _migrationLogService->getCurrentDbVersion(),
            _migrationLogService->getTargetDbVersion(),
            &_mainWindow
        );
        dlg.exec();
    }

    /**
     * @brief Handle about request
     *
     */
    void HelpMenuController::_onAboutRequested()
    {
        const auto version = QString::fromStdString(GithubConstants::version);
        const auto githubRepoUrl =
            QString::fromStdString(GithubConstants::getGithubRepoUrl());

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