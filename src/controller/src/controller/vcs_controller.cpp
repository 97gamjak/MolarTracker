#include "vcs_controller.hpp"

#include <QCoreApplication>
#include <QMessageBox>

#include "common/qt_helpers.hpp"
#include "settings/settings.hpp"
#include "ui/main_window.hpp"
#include "ui/update/download_progress_dialog.hpp"
#include "ui/update/update_available_dialog.hpp"
#include "vcs/update_check_service.hpp"
#include "vcs/update_installer.hpp"

namespace controller
{
    /**
     * @brief Construct a new VCSController::VCSController object
     *
     * @param mainWindow
     * @param settings
     */
    VCSController::VCSController(
        const std::shared_ptr<ui::MainWindow>&     mainWindow,
        const std::shared_ptr<settings::Settings>& settings
    )
        : _updateCheckService(std::make_unique<vcs::UpdateCheckService>()),
          _mainWindow(mainWindow),
          _settings(settings)
    {
        QObject::connect(
            _updateCheckService.get(),
            &vcs::UpdateCheckService::updateAvailable,
            _mainWindow.get(),
            [this](common::SemVer latest)
            {
                auto&       generalSettings = _settings->getGeneralSettings();
                const auto& dismissed =
                    generalSettings.getDismissedUpdateVersion().getOptional();

                if (dismissed.has_value() && *dismissed == latest.toString())
                    return;

                auto* dialog = common::makeQChild<ui::UpdateAvailableDialog>(
                    latest,
                    _mainWindow.get()
                );

                QObject::connect(
                    dialog,
                    &ui::UpdateAvailableDialog::downloadRequested,
                    _mainWindow.get(),
                    [this, dialog]()
                    {
                        dialog->accept();
                        _handleDownloadRequested();
                    }
                );

                dialog->exec();

                if (dialog->isDismissedForVersion())
                {
                    generalSettings.getDismissedUpdateVersion().set(
                        latest.toString()
                    );
                    _settings->save();
                }
            }
        );
    }

    VCSController::~VCSController() = default;

    /**
     * @brief Start the VCS controller, this function starts the update check
     * service to periodically check for newer releases on GitHub, and it also
     * sets up the necessary connections to handle update notifications and
     * user interactions with the update available dialog.
     */
    void VCSController::start() { _updateCheckService->start(); }

    /**
     * @brief Handle the user requesting to download and install an update.
     *
     * Shows a DownloadProgressDialog to fetch the matching release asset,
     * stages it via UpdateInstaller, asks the user to confirm a restart, then
     * applies the update and quits so the relaunched instance takes over.
     * Every failure stage (no compatible asset, download failed, staging
     * failed, apply failed) is surfaced via QMessageBox — nothing fails
     * silently.
     */
    void VCSController::_handleDownloadRequested()
    {
        auto* progressDialog =
            common::makeQChild<ui::DownloadProgressDialog>(_mainWindow.get());
        const auto outcome = progressDialog->exec();

        const auto& downloadResult = progressDialog->result();
        if (outcome != QDialog::Accepted)
        {
            if (downloadResult.has_value() && !downloadResult->has_value())
            {
                QMessageBox::warning(
                    _mainWindow.get(),
                    "Update Download Failed",
                    QString::fromStdString(downloadResult->error().toString())
                );
            }
            return;
        }

        const auto& archivePath = **downloadResult;

        const auto stageResult =
            vcs::UpdateInstaller::stageDownloadedAsset(archivePath);
        if (!stageResult)
        {
            QMessageBox::warning(
                _mainWindow.get(),
                "Update Failed",
                QString::fromStdString(stageResult.error().toString())
            );
            return;
        }

        const auto confirmed = QMessageBox::question(
            _mainWindow.get(),
            "Restart to Update",
            "The update has been downloaded. Restart MolarTracker now to "
            "install it?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (confirmed != QMessageBox::Yes)
            return;

        const auto applyResult =
            vcs::UpdateInstaller::applyUpdateAndRestart(*stageResult);
        if (!applyResult)
        {
            QMessageBox::warning(
                _mainWindow.get(),
                "Update Failed",
                QString::fromStdString(applyResult.error().toString())
            );
            return;
        }

        QCoreApplication::quit();
    }

}   // namespace controller