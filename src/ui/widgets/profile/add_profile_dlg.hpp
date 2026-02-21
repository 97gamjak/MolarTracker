#ifndef __UI__WIDGETS__PROFILE__ADD_PROFILE_DLG_HPP__
#define __UI__WIDGETS__PROFILE__ADD_PROFILE_DLG_HPP__

#include <QDialog>

#include "ui/widgets/validators/email_line_edit.hpp"
#include "ui/widgets/validators/name_line_edit.hpp"

namespace drafts
{
    struct ProfileDraft;   // Forward declaration
}

namespace app
{
    class ProfileStore;   // Forward declaration
}

namespace settings
{
    class Settings;   // Forward declaration
}

class QLineEdit;     // Forward declaration
class QPushButton;   // Forward declaration
class QCheckBox;     // Forward declaration
class QVBoxLayout;   // Forward declaration

namespace ui
{
    class UndoStack;   // Forward declaration

    /**
     * @brief Dialog for adding a new profile.
     *
     */
    class AddProfileDialog : public QDialog
    {
        Q_OBJECT

       private:
        /// Reference to the profile store
        app::ProfileStore& _profileStore;
        /// Reference to the settings
        settings::Settings& _settings;
        /// Reference to the undo stack
        UndoStack& _undoStack;

        /// Whether to enforce a default profile
        bool _enforceDefaultProfile = false;

        /// Flag to prevent emitting cancel action multiple times when the
        /// dialog is closed without adding a profile
        bool _canBeClosed = true;

        /// ptr to the main layout of the dialog
        QVBoxLayout* _mainLayout = nullptr;
        /// Line edit for the profile name
        NameLineEdit* _nameLineEdit = nullptr;
        /// Line edit for the profile email
        EmailLineEdit* _emailLineEdit = nullptr;
        /// Button to add the profile
        QPushButton* _addButton = nullptr;
        /// Button to cancel the operation
        QPushButton* _cancelButton = nullptr;
        /// Checkbox to set the profile as active
        QCheckBox* _setActiveCheckBox = nullptr;
        /// Checkbox to set the profile as default
        QCheckBox* _setAsDefaultCheckBox = nullptr;

       public:
        explicit AddProfileDialog(
            app::ProfileStore&  profileStore,
            settings::Settings& settings,
            UndoStack&          undoStack,
            bool                canBeClosed = true,
            QWidget*            parent      = nullptr
        );

        void setEnforceDefaultProfile(bool value);

        [[nodiscard]] bool isActiveChecked() const;
        [[nodiscard]] bool isDefaultChecked() const;

        void showNameAlreadyExistsError();

        /**
         * @brief Enum class for actions in the add profile dialog
         *
         */
        enum class Action
        {
            Ok,
            Cancel
        };

       signals:
        /**
         * @brief QT signal for when an action is performed in the add profile
         * dialog
         *
         * @param action
         * @param profile
         */
        void requested(
            const Action&               action,
            const drafts::ProfileDraft& profile
        );

       protected:
        void reject() override;

       private:
        void _buildUI();
        void _buildFormSection();
        void _buildToggleSection();
        void _buildButtonSection();

        void _updateToggleStates();

        [[nodiscard]] struct drafts::ProfileDraft _getProfile() const;

        void _emit(const Action& action);
        void _emitOk();
        void _emitCancel();
    };

}   // namespace ui

#endif   // __UI__WIDGETS__PROFILE__ADD_PROFILE_DLG_HPP__