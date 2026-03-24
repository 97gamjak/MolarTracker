#ifndef __CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__
#define __CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__

#include <QObject>
#include <QPointer>

#include "ui/commands/commands.hpp"
#include "ui/widgets/profile/add_profile_dlg.hpp"
#include "ui/widgets/profile/profile_selection_dlg.hpp"

class QMainWindow;   // Forward declaration

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace drafts
{
    struct ProfileDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{
    class UndoStack;   // Forward declaration
}   // namespace ui

namespace controller
{
    /**
     * @brief Controller to ensure a profile exists on application startup
     *
     */
    class EnsureProfileController : public QObject
    {
        Q_OBJECT
       private:
        /// Maximum number of times to check for a valid profile before giving
        /// up and showing an error message, this is to prevent infinite loops
        /// in case of unexpected issues with profile creation or selection
        static constexpr std::size_t MAX_PROFILE_CHECKS = 10;

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the application context
        app::AppContext& _appContext;
        /// Reference to the undo stack
        ui::UndoStack& _undoStack;

        /// Pointer to the profile selection dialog
        QPointer<ui::ProfileSelectionDialog> _profileSelectionDialog;
        /// Pointer to the add profile dialog
        QPointer<ui::AddProfileDialog> _addProfileDialog;

        /// Command for ensuring profile existence, this command will contain
        /// the sub-commands for setting the default profile, setting the active
        /// profile and adding a new profile if needed
        ui::Commands _ensureProfileExistsCommand{
            "Ensure Profile Exists Command"
        };

       public:
        explicit EnsureProfileController(
            QMainWindow&     mainWindow,
            app::AppContext& appContext,
            ui::UndoStack&   undoStack
        );

        void ensureProfileExists();

       private slots:
        void _onProfileSelectionRequested(
            const ui::ProfileSelectionDialog::Action& action,
            const std::string&                        profileName
        );
        void _onAddProfileRequested(
            const ui::AddProfileDialog::Action& action,
            const drafts::ProfileDraft&         profileDraft
        );

       private:
        void _defaultProfileExists(const std::string& defaultProfile);
        void _noDefaultProfile();
        void _showAddProfileDialog();
        void _showProfileSelectionDialog();
        bool _activateProfile(const std::string& name);

        static void _fatalError(const std::string& message);
    };

}   // namespace controller

#endif   // __CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__