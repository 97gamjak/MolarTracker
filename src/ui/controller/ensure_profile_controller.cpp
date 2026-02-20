#include "ensure_profile_controller.hpp"

#include <QMainWindow>
#include <QMessageBox>
#include <QObject>
#include <QStatusBar>
#include <format>

#include "app/app_context.hpp"
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
     * @param appContext
     * @param undoStack
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
        const auto& settings = _appContext.getSettings().getGeneralSettings();
        auto&       profileStore = _appContext.getStore().getProfileStore();

        std::size_t counter = 0;

        while (!settings.hasDefaultProfile() || !profileStore.hasActiveProfile()
        )
        {
            std::optional<std::string_view> name = settings.getDefaultProfile();

            if (name.has_value() && profileStore.profileExists(name.value()))
            {
                profileStore.setActiveProfile(name);
                auto* statusBar = _mainWindow.statusBar();

                const auto msg = "Default profile '" +
                                 std::string(name.value()) +
                                 "' loaded successfully.";

                showInfoStatusBar(LOG_INFO_OBJECT(msg), statusBar);

                break;
            }
            else if (name.has_value())
                _defaultProfileExists();
            else
                _noDefaultProfile();

            // if we reach this point, it means that the profile selection or
            // creation process was completed, but we still don't have a valid
            // profile loaded. This could happen if there are unexpected issues
            // with the profile store or if the user somehow manages to delete
            // the default profile during the selection/creation process. To
            // prevent infinite loops in such cases, we keep track of how many
            // times we've checked for a valid profile and if it exceeds a
            // certain threshold, we show a fatal error message and stop trying
            // to ensure profile existence.
            if (++counter >= MAX_PROFILE_CHECKS)
            {
                ExceptionDialog::showFatal(
                    "Profile Load Failed",
                    LOG_ERROR_OBJECT(
                        "Failed to load a valid profile after " +
                        std::to_string(MAX_PROFILE_CHECKS) +
                        " attempts. Please check the profile store for issues."
                    )
                );
            }
        }
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
        auto& settings     = _appContext.getSettings().getGeneralSettings();

        const auto name = settings.getDefaultProfile().value();
        // we unset the default profile name to prevent infinite loops in
        // case the profile store keeps failing to load the profile for some
        // reason. This way, the next time ensureProfileExists is called, it
        // will go through the _noDefaultProfile path instead of trying to
        // load the non-existent default profile again.
        settings.unsetDefaultProfile();

        showWarningMessageBox(
            "Default Profile Not Found",
            LOG_WARNING_OBJECT(
                std::format(
                    "The default profile '{}' could not be found. It "
                    "might have been deleted or there might be an issue "
                    "with the profile store. Please select an existing "
                    "profile or create a new one to continue.",
                    name
                )
            )
        );

        if (profileStore.hasProfiles())
            _showProfileSelectionDialog();
        else
            _showAddProfileDialog();
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
                "No Default Profile Configured - No default profile is "
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
     * @brief Show the Add Profile Dialog to create a new profile.
     *
     */
    void EnsureProfileController::_showAddProfileDialog()
    {
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _addProfileDialog = new AddProfileDialog{
            _appContext.getStore().getProfileStore(),
            _appContext.getSettings(),
            _undoStack,
            false,   // canBeClosed = false to disable the close button
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

        if (auto* dialog = _addProfileDialog.data())
            dialog->exec();
    }

    /**
     * @brief Show the Profile Selection Dialog to select an existing profile.
     *
     */
    void EnsureProfileController::_showProfileSelectionDialog()
    {
        const auto& profileStore = _appContext.getStore().getProfileStore();

        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
        _profileSelectionDialog = new ProfileSelectionDialog{
            &_mainWindow,
            profileStore.getAllProfileNames(),
            false   // canBeClosed = false to disable the close button
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

        if (auto* dialog = _profileSelectionDialog.data())
            dialog->exec();
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
        if (action == ProfileSelectionDialog::Action::Ok)
        {
            _appContext.getStore().getProfileStore().setActiveProfile(
                profileName
            );
            _appContext.getSettings().getGeneralSettings().setDefaultProfile(
                profileName
            );

            const auto msg =
                "Profile '" + profileName + "' selected successfully.";

            showInfoStatusBar(
                LOG_INFO_OBJECT(
                    "Profile '" + profileName + "' selected successfully."
                ),
                _mainWindow.statusBar()
            );
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
        if (action == AddProfileDialog::Action::Ok)
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
                        const auto errorMsg =
                            "Failed to add profile: " + addError->getMessage();

                        ExceptionDialog::showFatal(
                            "Add Profile Error",
                            LOG_ERROR_OBJECT(errorMsg)
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

                if (auto* dialog = _addProfileDialog.data())
                    dialog->accept();
            }
        }
    }

}   // namespace ui