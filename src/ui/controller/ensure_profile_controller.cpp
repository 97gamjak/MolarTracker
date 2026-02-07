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
#include "ui/widgets/utils/infos.hpp"
#include "ui/widgets/utils/warnings.hpp"

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

    /**
     * @brief Ensure that a profile exists. If a default profile is configured,
     * it will check if it exists and load it. If it does not exist, it will
     * show a warning message and prompt the user to select an existing profile
     * or create a new one. If no default profile is configured, it will show a
     * warning message and prompt the user to select an existing profile or
     * create a new one.
     *
     */
    void EnsureProfileController::ensureProfileExists()
    {
        const auto& settings = _appContext.getSettings();

        if (settings.hasDefaultProfile())
            _defaultProfileExists();
        else
            _noDefaultProfile();
    }

    /**
     * @brief Check if the default profile exists and load it. If it does not
     * exist, show a warning message and prompt the user to select an existing
     * profile or create a new one.
     *
     */
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

            showWarningMessageBox(
                "Default Profile Not Found",
                LOG_WARNING_OBJECT(
                    std::format(
                        "Default Profile not Found The default profile '{}' "
                        "could not be found. It might have been deleted or "
                        "there there might be an issue with the profile store. "
                        "You will need to select an existing profile or create "
                        "a new one to continue.",
                        name
                    )
                )
            );

            if (profileStore.hasProfiles())
                _showProfileSelectionDialog();
            else
                _showAddProfileDialog();
        }
        else   // everything is fine!
        {
            profileStore.setActiveProfile(name);

            showInfoStatusBar(
                LOG_INFO_OBJECT(
                    "Default profile '" + name + "' loaded successfully."
                ),
                statusBar
            );

            // we relaunch to ensure all components are properly
            // initialized with the active profile. This is
            // necessary because some components might have been
            // initialized before the active profile was set,
            // which can lead to issues if they try to access the
            // active profile during their initialization.
            _relaunch();
        }
    }

    /**
     * @brief Handle the case when no default profile is configured. Show a
     * warning message and prompt the user to select an existing profile or
     * create a new one.
     *
     */
    void EnsureProfileController::_noDefaultProfile()
    {
        auto& profileStore = _appContext.getStore().getProfileStore();

        showWarningMessageBox(
            "No Default Profile Configured",
            LOG_WARNING_OBJECT(
                "No Default Profile Configured No default profile is "
                "configured. You will need to select an existing profile or "
                "create a new one to continue."
            )
        );

        if (profileStore.hasProfiles())
            _showProfileSelectionDialog();
        else
            _showAddProfileDialog();
    }

    /**
     * @brief Relaunch the application to ensure all components are properly
     * initialized with the active profile. This is necessary because some
     * components might have been initialized before the active profile was set,
     * which can lead to issues if they try to access the active profile during
     * their initialization.
     *
     */
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

            showWarningMessageBox(
                "Failed to Ensure Profile Existence",
                LOG_WARNING_OBJECT(
                    "Failed to Ensure Profile Existence No active profile "
                    "found "
                    "after ensuring profile existence. This should not happen, "
                    "as "
                    "the ensureProfileExists method should have set an active "
                    "profile. Trying to ensure profile existence again. If you "
                    "keep seeing this message, please report it to the "
                    "developers "
                    "under " +
                    Constants::getGithubIssuesUrl()
                )
            );

            ensureProfileExists();
        }
    }

    /**
     * @brief Show the Add Profile Dialog to create a new profile.
     *
     */
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

    /**
     * @brief Show the Profile Selection Dialog to select an existing profile.
     *
     */
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

    /**
     * @brief Handle the profile selection result from the Profile Selection
     * Dialog. If the user selected a profile, set it as the active profile and
     * relaunch the application. If the user canceled the selection, show a
     * warning message and try to ensure profile existence again.
     *
     * @param action The action performed by the user (Ok or Cancel).
     * @param profileName The name of the selected profile (only valid if action
     * is Ok).
     */
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

                showWarningMessageBox(
                    "No Profile Selected",
                    LOG_WARNING_OBJECT(
                        "No profile selected. Application cannot continue "
                        "without "
                        "an active profile. Trying to ensure profile existence "
                        "again. If you keep seeing this message, please report "
                        "it "
                        "to the developers under " +
                        Constants::getGithubIssuesUrl()
                    )
                );

                _relaunch();
                break;
            }
        }
    }

    /**
     * @brief Handle the result from the Add Profile Dialog. If the user
     * created a profile, set it as the active profile and relaunch the
     * application. If the user canceled the creation, show a warning message
     * and try to ensure profile existence again.
     *
     * @param action The action performed by the user (Ok or Cancel).
     * @param profileDraft The draft of the created profile (only valid if
     * action is Ok).
     *
     * Note: The actual profile creation is handled by the AddProfileCommand,
     * which is executed when the user clicks Ok in the AddProfileDialog. The
     * command will add the profile to the profile store and set it as active if
     * the "Set as active" option is checked in the dialog. If the command fails
     * for some reason (e.g., a profile with the same name already exists), it
     * will show an error message in the dialog and not close it, allowing the
     * user to fix the issue and try again. If the command succeeds, it will
     * show a success message in the status bar and close the dialog. After
     * that, it will call _relaunch() to ensure all components are properly
     * initialized with the new active profile. If the user clicks Cancel in the
     * AddProfileDialog, it will show a warning message and call _relaunch() to
     * try to ensure profile existence again, which will likely show the
     * AddProfileDialog again since there will still be no active profile. This
     * is to prevent the application from being in a state where it cannot
     * continue without an active profile. This flow ensures that the user is
     * guided to create or select a profile if there is no active profile, and
     * that the application can continue functioning properly once an active
     * profile is set.
     */
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
                    showInfoStatusBar(
                        LOG_INFO_OBJECT(
                            "Profile '" + profileDraft.name +
                            "' created successfully."
                        ),
                        _mainWindow.statusBar()
                    );

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