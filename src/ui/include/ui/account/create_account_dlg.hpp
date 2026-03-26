#ifndef __UI__INCLUDE__UI__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__
#define __UI__INCLUDE__UI__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__

#include <QDialog>

class QVBoxLayout;   // Forward declaration
class QComboBox;     // Forward declaration

namespace drafts
{
    struct AccountDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{
    class NameLineEdit;   // Forward declaration

    /**
     * @brief Dialog for creating a new account, this dialog will be used to
     * gather the necessary information from the user to create a new account,
     * such as the account name, type, and currency, once the user has provided
     * the necessary information and confirmed the creation of the account, this
     * dialog will emit a signal with the account draft containing the provided
     * information, which can then be used by the controller to create the
     * actual account in the store
     *
     */
    class CreateAccountDialog : public QDialog
    {
        Q_OBJECT

       private:
        /// Line edit for the profile name
        NameLineEdit* _nameLineEdit = nullptr;

        /// Combo box for the account type (e.g. cash, security, etc.)
        QComboBox* _accountType = nullptr;
        /// Combo box for the account currency
        QComboBox* _currency = nullptr;

        /// Button to confirm the creation of the account
        QPushButton* _addButton = nullptr;
        /// Button to cancel the creation of the account
        QPushButton* _cancelButton = nullptr;

       public:
        explicit CreateAccountDialog(QWidget* parent);

       signals:
        /**
         * @brief QT signal for when an action is performed in the create
         * account dlg
         *
         * @param account
         */
        void requested(const drafts::AccountDraft& account);

       private:
        void _buildUI();

        void _buildFormLayout(QVBoxLayout* parent);
        void _buildButtonSection(QVBoxLayout* parent);

        void _emitOk();

        drafts::AccountDraft _getAccount();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__