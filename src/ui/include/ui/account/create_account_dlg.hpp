#ifndef __UI__INCLUDE__UI__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__
#define __UI__INCLUDE__UI__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "ui/base/dialog.hpp"

class QVBoxLayout;   // Forward declaration
class QComboBox;     // Forward declaration
class QLabel;        // Forward declaration

namespace drafts
{
    class AccountDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{
    class NameLineEdit;   // Forward declaration
    class AccountCombo;   // Forward declaration

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
    class CreateAccountDialog : public Dialog
    {
        Q_OBJECT

       private:
        /// Line edit for the profile name
        NameLineEdit* _nameLineEdit = nullptr;

        /// Combo box for the account type (e.g. cash, security, etc.)
        QComboBox* _accountType = nullptr;
        /// Combo box for the account currency
        QComboBox* _currency = nullptr;
        /// Combo box for selecting a reference account
        AccountCombo* _referenceAccount = nullptr;
        /// Label for the reference account combo box
        QLabel* _referenceAccountLabel = nullptr;

        /// Button to confirm the creation of the account
        QPushButton* _addButton = nullptr;
        /// Button to cancel the creation of the account
        QPushButton* _cancelButton = nullptr;

        /// The list of available accounts
        std::vector<drafts::AccountDraft> _availableCashAccounts;
        /// The list of available security accounts
        std::vector<drafts::AccountDraft> _availableSecurityAccounts;

       public:
        explicit CreateAccountDialog(
            const std::vector<drafts::AccountDraft>& availableAccounts,
            QWidget*                                 parent
        );

       signals:
        /**
         * @brief QT signal for when an action is performed in the create
         * account dlg
         *
         * @param account
         */
        void requested(
            const drafts::AccountDraft& account,
            std::optional<AccountId>    referenceAccount
        );

       private:
        void _buildUI();

        void _buildFormLayout(QVBoxLayout* parent);
        void _buildButtonSection(QVBoxLayout* parent);

        void _emitOk();

        [[nodiscard]]
        drafts::AccountDraft _getAccount() const;
        [[nodiscard]]
        std::optional<AccountId> _getSelectedReferenceAccount() const;

        void _updateReferenceAccountVisibility();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__ACCOUNT__CREATE_ACCOUNT_DLG_HPP__
