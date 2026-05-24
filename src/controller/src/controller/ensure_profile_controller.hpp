#ifndef __CONTROLLER__SRC__CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__
#define __CONTROLLER__SRC__CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__

#include <QObject>
#include <QPointer>
#include <memory>

class QMainWindow;   // Forward declaration

namespace app
{
    class AppContext;   // Forward declaration
}   // namespace app

namespace drafts
{
    class ProfileDraft;   // Forward declaration
}   // namespace drafts

namespace cmd
{
    class UndoStack;   // Forward declaration
}   // namespace cmd

namespace ui
{
    class AddProfileDialog;         // Forward declaration
    class ProfileSelectionDialog;   // Forward declaration

    enum class AddProfileDialogAction : std::uint8_t;   // Forward declaration
    enum class ProfileSelectionDialogAction : std::uint8_t;   // Forward
                                                              // declaration

}   // namespace ui

namespace cmd
{
    class Commands;   // Forward declaration
}   // namespace cmd

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
        cmd::UndoStack& _undoStack;

        /// Pointer to the profile selection dialog
        QPointer<ui::ProfileSelectionDialog> _profileSelectionDialog;
        /// Pointer to the add profile dialog
        QPointer<ui::AddProfileDialog> _addProfileDialog;

        /// Command for ensuring profile existence, this command will contain
        /// the sub-commands for setting the default profile, setting the active
        /// profile and adding a new profile if needed
        std::unique_ptr<cmd::Commands> _ensureProfileExistsCommand;

       public:
        explicit EnsureProfileController(
            QMainWindow&     mainWindow,
            app::AppContext& appContext,
            cmd::UndoStack&  undoStack
        );

        ~EnsureProfileController() override;

        void ensureProfileExists();

       private slots:
        void _onProfileSelectionRequested(
            const ui::ProfileSelectionDialogAction& action,
            const std::string&                      profileName
        );
        void _onAddProfileRequested(
            const ui::AddProfileDialogAction& action,
            const drafts::ProfileDraft&       profileDraft
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

#endif   // __CONTROLLER__SRC__CONTROLLER__ENSURE_PROFILE_CONTROLLER_HPP__