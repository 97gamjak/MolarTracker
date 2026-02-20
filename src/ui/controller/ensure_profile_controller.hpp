#ifndef __UI__CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__
#define __UI__CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__

#include <QObject>
#include <QPointer>

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
}

// TODO: implement this via a settings value that has a max number of checks
// predefined
constexpr std::size_t MAX_PROFILE_CHECKS = 10;

namespace ui
{
    class UndoStack;   // Forward declaration

    /**
     * @brief Controller to ensure a profile exists on application startup
     *
     */
    class EnsureProfileController : public QObject
    {
        Q_OBJECT

       private:
        /// Reference to the main window
        QMainWindow& _mainWindow;
        /// Reference to the application context
        app::AppContext& _appContext;
        /// Reference to the undo stack
        UndoStack& _undoStack;

        /// Counter for the number of profile checks
        std::size_t _callCount = MAX_PROFILE_CHECKS;
        /// Pointer to the profile selection dialog
        QPointer<ProfileSelectionDialog> _profileSelectionDialog;
        /// Pointer to the add profile dialog
        QPointer<AddProfileDialog> _addProfileDialog;

       public:
        explicit EnsureProfileController(
            QMainWindow&     mainWindow,
            app::AppContext& appContext,
            UndoStack&       undoStack
        );

        void ensureProfileExists();

       private slots:
        void _onProfileSelectionRequested(
            const ProfileSelectionDialog::Action& action,
            const std::string&                    profileName
        );
        void _onAddProfileRequested(
            const AddProfileDialog::Action& action,
            const drafts::ProfileDraft&     profileDraft
        );

       private:
        void _defaultProfileExists();
        void _noDefaultProfile();
        void _relaunch();
        void _showAddProfileDialog();
        void _showProfileSelectionDialog();
    };

}   // namespace ui

#endif   // __UI__CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__