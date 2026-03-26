#ifndef __UI__INCLUDE__UI__PROFILE__ADD_PROFILE_DLG_HPP__
#define __UI__INCLUDE__UI__PROFILE__ADD_PROFILE_DLG_HPP__

#include <QDialog>

#include "ui/validators/email_line_edit.hpp"
#include "ui/validators/name_line_edit.hpp"

namespace drafts
{
    struct ProfileDraft;   // Forward declaration
}   // namespace drafts

class QLineEdit;     // Forward declaration
class QPushButton;   // Forward declaration
class QCheckBox;     // Forward declaration
class QVBoxLayout;   // Forward declaration

namespace ui
{
    /**
     * @brief Dialog for adding a new profile.
     *
     */
    class AddProfileDialog : public QDialog
    {
        Q_OBJECT

       public:
        struct Settings;

       private:
        /// Settings for the add profile dialog
        std::shared_ptr<Settings> _settings;

        /// Whether to enforce a default profile
        bool _enforceDefaultProfile = false;

        /// Flag to prevent emitting cancel action multiple times when the
        /// dialog is closed without adding a profile
        bool _canBeClosed = true;

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
            std::shared_ptr<Settings> settings,
            bool                      canBeClosed,
            QWidget*                  parent
        );
        explicit AddProfileDialog(
            std::shared_ptr<Settings> settings,
            QWidget*                  parent
        );

        void setEnforceDefaultProfile(bool value);

        [[nodiscard]] bool isActiveChecked() const;
        [[nodiscard]] bool isDefaultChecked() const;

        void showNameAlreadyExistsError();

        /**
         * @brief Enum class for actions in the add profile dialog
         *
         */
        enum class Action : std::uint8_t
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
        void _buildFormSection(QVBoxLayout* parent);
        void _buildToggleSection(QVBoxLayout* parent);
        void _buildButtonSection(QVBoxLayout* parent);

        void _updateToggleStates();

        [[nodiscard]] struct drafts::ProfileDraft _getProfile() const;

        void _emit(const Action& action);
        void _emitOk();
        void _emitCancel();
    };

    /**
     * @brief Settings for AddProfileDialog
     *
     */
    struct AddProfileDialog::Settings
    {
        /// The size of the dialog (width, height)
        std::pair<int, int> dialogSize;

        Settings() = delete;
        explicit Settings(std::pair<int, int> _dialogSize);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__PROFILE__ADD_PROFILE_DLG_HPP__