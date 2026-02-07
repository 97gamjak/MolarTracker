#include "ensure_profile_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QStatusBar>

#include "app/app_context.hpp"
#include "config/constants.hpp"
#include "logging/logging_base.hpp"
#include "ui/commands/add_profile_command.hpp"
#include "ui/commands/add_profile_command_error.hpp"
#include "ui/commands/undo_stack.hpp"
#include "ui/widgets/exceptions/exception_dialog.hpp"
#include "ui/widgets/profile/add_profile_dlg.hpp"
#include "ui/widgets/profile/profile_selection_dlg.hpp"

#define __LOG_CATEGORY__ LogCategory::ui_ensureProfileController
#include "logging/log_macros.hpp"

namespace ui
{
    /**
     * @brief Construct a new Ensure Profile Controller:: Ensure Profile
     * Controller object
     *
     * @param mainWindow
     */
    EnsureProfileController::EnsureProfileController(
        QMainWindow&     mainWindow,
        app::AppContext& appContext,
        UndoStack&       undoStack
    )
        : QObject(&mainWindow),
          _mainWindow(mainWindow),
          _appContext(appContext),
          _undoStack(undoStack)
    {
    }

    void EnsureProfileController::ensureProfileExists()
    {
        const auto& settings = _appContext.getSettings();

        if (settings.hasDefaultProfile())
            _defaultProfileExists();
        else
            _noDefaultProfile();
    }

    void EnsureProfileController::_defaultProfileExists()
    {
        auto& profileStore = _appContext.getStore().getProfileStore();
        auto& settings     = _appContext.getSettings();
        auto* statusBar    = _mainWindow.statusBar();

        const auto name = settings.getDefaultProfileName().value();
        if (!profileStore.profileExists(name))
        {
            // we unset the default profile name to prevent infinite loops in
            // case the profile store keeps failing to load the profile for some
            // reason. This way, the next time ensureProfileExists is called, it
            // will go through the _noDefaultProfile path instead of trying to
            // load the non-existent default profile again.
            settings.unsetDefaultProfileName();

            const auto msg =
                "Default profile '" + name + "' not found in database.";

            LOG_WARNING(msg);
            if (statusBar)
                statusBar->showMessage(QString::fromStdString(msg));

            if (profileStore.hasProfiles())
                _showProfileSelectionDialog();
            else
                _showAddProfileDialog();
        }
        else   // everything is fine!
        {
            profileStore.setActiveProfile(name);

            const auto msg = "Loaded default profile: " + name;
            LOG_INFO(msg);

            if (statusBar)
                statusBar->showMessage(QString::fromStdString(msg));

            // we relaunch to ensure all components are properly
            // initialized with the active profile. This is
            // necessary because some components might have been
            // initialized before the active profile was set,
            // which can lead to issues if they try to access the
            // active profile during their initialization.
            _relaunch();
        }
    }

    void EnsureProfileController::_noDefaultProfile()
    {
        auto& profileStore = _appContext.getStore().getProfileStore();
        auto* statusBar    = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("No default profile configured.");

        if (profileStore.hasProfiles())
            _showProfileSelectionDialog();
        else
            _showAddProfileDialog();
    }

    void EnsureProfileController::_relaunch()
    {
        const auto& profileStore = _appContext.getStore().getProfileStore();
        const auto& settings     = _appContext.getSettings();

        if (!settings.hasDefaultProfile() || !profileStore.hasActiveProfile() ||
            !profileStore.getActiveProfile().has_value())
        {
            _callCount--;
            if (_callCount == 0)
            {
                const auto errorMsg =
                    "Failed to ensure profile existence after multiple "
                    "attempts. "
                    "This should not happen, as the ensureProfileExists method "
                    "should have set an active profile. Please report this to "
                    "the "
                    "developers under " +
                    Constants::getGithubIssuesUrl();
                LOG_ERROR(errorMsg);

                ExceptionDialog::showFatal(
                    "Ensure Profile Error",
                    QString::fromStdString(errorMsg)
                );

                _callCount = MAX_PROFILE_CHECKS;   // reset call count for
                                                   // future attempts
            }

            LOG_WARNING(
                "No active profile found after ensuring profile existence. "
                "This should not happen, as the ensureProfileExists method "
                "should have set an active profile. If you are sure you did "
                "everything right, please report this to the developers "
                "under " +
                Constants::getGithubIssuesUrl()
            );

            ensureProfileExists();
        }
    }

    void EnsureProfileController::_showAddProfileDialog()
    {
        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("No profiles found.");

        _addProfileDialog = new AddProfileDialog{
            _appContext.getStore().getProfileStore(),
            _appContext.getSettings(),
            _undoStack,
            &_mainWindow
        };

        connect(
            _addProfileDialog,
            &AddProfileDialog::requested,
            this,
            &EnsureProfileController::_onAddProfileRequested
        );

        _addProfileDialog->setWindowTitle("Create your first profile");
        _addProfileDialog->setModal(true);
        _addProfileDialog->setAttribute(Qt::WA_DeleteOnClose);
        _addProfileDialog->setEnforceDefaultProfile(true);

        _addProfileDialog->show();
        _addProfileDialog->raise();
        _addProfileDialog->activateWindow();
    }

    void EnsureProfileController::_showProfileSelectionDialog()
    {
        auto* statusBar = _mainWindow.statusBar();

        if (statusBar)
            statusBar->showMessage("Select a profile to continue.");

        const auto& profileStore = _appContext.getStore().getProfileStore();

        _profileSelectionDialog = new ProfileSelectionDialog{
            &_mainWindow,
            profileStore.getAllProfileNames()
        };

        connect(
            _profileSelectionDialog,
            &ProfileSelectionDialog::requested,
            this,
            &EnsureProfileController::_onProfileSelectionRequested
        );

        _profileSelectionDialog->setWindowTitle("Select a profile");
        _profileSelectionDialog->setModal(true);
        _profileSelectionDialog->setAttribute(Qt::WA_DeleteOnClose);

        _profileSelectionDialog->show();
        _profileSelectionDialog->raise();
        _profileSelectionDialog->activateWindow();
    }

    void EnsureProfileController::_onProfileSelectionRequested(
        const ProfileSelectionDialog::Action& action,
        const std::string&                    profileName
    )
    {
        switch (action)
        {
            case ProfileSelectionDialog::Action::Ok:
            {
                _appContext.getStore().getProfileStore().setActiveProfile(
                    profileName
                );
                _appContext.getSettings().setDefaultProfileName(profileName);

                LOG_INFO("Selected profile: " + profileName);

                _profileSelectionDialog->close();

                _relaunch();
                break;
            }

            case ProfileSelectionDialog::Action::Cancel:
            {
                _profileSelectionDialog->close();

                LOG_WARNING(
                    "No profile selected. Application cannot continue without "
                    "an active profile. Trying to ensure profile existence "
                    "again. If you keep seeing this message, please report it "
                    "to the developers under " +
                    Constants::getGithubIssuesUrl()
                );

                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setWindowTitle("No Profile Selected");
                msgBox.setText(
                    "You must select a profile to continue. Please select a "
                    "profile to proceed."
                );
                msgBox.setStandardButtons(QMessageBox::Ok);

                msgBox.exec();

                _relaunch();
                break;
            }
        }
    }

    void EnsureProfileController::_onAddProfileRequested(
        const AddProfileDialog::Action& action,
        const drafts::ProfileDraft&     profileDraft
    )
    {
        switch (action)
        {
            case AddProfileDialog::Action::Ok:
            {
                // we don't need to do anything here, as the AddProfileCommand
                // already adds the profile and sets it as active if needed.
                const auto result = _undoStack.makeAndDo<AddProfileCommand>(
                    _appContext.getStore().getProfileStore(),
                    _appContext.getSettings(),
                    profileDraft,
                    _addProfileDialog->isActiveChecked(),
                    _addProfileDialog->isDefaultChecked()
                );

                if (!result)
                {
                    const auto& addProfileError = result.error();
                    auto*       errorPtr        = addProfileError.get();

                    if (auto* addError =
                            dynamic_cast<AddProfileCommandError*>(errorPtr))
                    {
                        if (addError->getCode() ==
                            AddProfileCommandErrorCode::NameAlreadyExists)
                        {
                            _addProfileDialog->showNameAlreadyExistsError();
                        }
                        else
                        {
                            const auto errorMsg = "Failed to add profile: " +
                                                  addError->getMessage();

                            LOG_ERROR(errorMsg);

                            ExceptionDialog::showFatal(
                                "Add Profile Error",
                                QString::fromStdString(errorMsg)
                            );
                        }
                    }
                }
                else
                {
                    auto*      statusBar = _mainWindow.statusBar();
                    const auto msg       = "Profile '" + profileDraft.name +
                                     "' created successfully.";
                    if (statusBar)
                        statusBar->showMessage(QString::fromStdString(msg));

                    _addProfileDialog->close();
                }

                _relaunch();
                break;
            }

            case AddProfileDialog::Action::Cancel:
            {
                LOG_WARNING(
                    "No profile created. Application cannot continue without "
                    "an active profile. Trying to ensure profile existence "
                    "again. If you keep seeing this message, please report it "
                    "to the developers under " +
                    Constants::getGithubIssuesUrl()
                );
                _addProfileDialog->close();
                _relaunch();
                break;
            }
        }
    }

}   // namespace ui