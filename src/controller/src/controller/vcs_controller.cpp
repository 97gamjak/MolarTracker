#include "vcs_controller.hpp"

#include "common/qt_helpers.hpp"
#include "settings/settings.hpp"
#include "ui/main_window.hpp"
#include "ui/update/update_available_dialog.hpp"
#include "vcs/update_check_service.hpp"

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

}   // namespace controller