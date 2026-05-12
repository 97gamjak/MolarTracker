#ifndef __UI__INCLUDE__UI__PROFILE__PROFILE_SELECTION_DLG_HPP__
#define __UI__INCLUDE__UI__PROFILE__PROFILE_SELECTION_DLG_HPP__

#include <string>
#include <vector>

#include "ui/base/dialog.hpp"

class QWidget;            // Forward declaration
class QListWidget;        // Forward declaration
class QDialogButtonBox;   // Forward declaration

namespace ui
{
    /**
     * @brief enum for the actions that can be emitted by the dialog
     *
     */
    enum class ProfileSelectionDialogAction : std::uint8_t
    {
        Ok,
        Cancel
    };

    /**
     * @brief Dialog for selecting a profile from existing profiles.
     *
     */
    class ProfileSelectionDialog : public Dialog
    {
        Q_OBJECT

       private:
        /// List widget for displaying available profiles
        QListWidget* _profileListWidget = nullptr;
        /// Button box for dialog actions
        QDialogButtonBox* _buttonBox = nullptr;

        /// Flag to prevent emitting cancel action multiple times when the
        /// dialog is closed without selecting a profile
        bool _canBeClosed = true;

       public:
        explicit ProfileSelectionDialog(
            QWidget*                        parent,
            const std::vector<std::string>& profiles,
            bool                            canBeClosed
        );
        explicit ProfileSelectionDialog(
            QWidget*                        parent,
            const std::vector<std::string>& profiles
        );

       signals:
        /// Signal emitted when the user performs an action in the dialog
        void requested(
            const ProfileSelectionDialogAction& action,
            const std::string&                  profileName
        );

       protected:
        void reject() override;

       private:
        void _buildUI(const std::vector<std::string>& profiles);

        void _emit(
            const ProfileSelectionDialogAction& action,
            const std::string&                  profileName
        );
        void _emit(const ProfileSelectionDialogAction& action);
        void _emitOk();
        void _emitCancel();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__PROFILE__PROFILE_SELECTION_DLG_HPP__