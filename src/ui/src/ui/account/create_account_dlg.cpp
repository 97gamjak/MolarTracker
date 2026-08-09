#include "ui/account/create_account_dlg.hpp"

#include <qlabel.h>

#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "common/finance.hpp"
#include "common/qt_helpers.hpp"
#include "config/id_types.hpp"
#include "drafts/account_draft.hpp"
#include "ui/utils/account_combo.hpp"
#include "ui/validators/name_line_edit.hpp"
#include "ui/validators/validators.hpp"

namespace ui
{
    namespace
    {
        /**
         * @brief Get the selected account type from the given combo box, this
         * will take the current text of the combo box, convert it to an
         * AccountKind enum value, and return it, this allows the dialog to
         * easily retrieve the selected account type from the form without
         * having to manually parse the string value.
         *
         * @param comboBox The combo box containing the account type options
         * @return AccountKind The selected account type as an enum value
         */
        [[nodiscard]]
        AccountKind _getSelectedAccountType(QComboBox* comboBox)
        {
            const auto typeStr = comboBox->currentText().toStdString();
            return AccountKindMeta::from_string(typeStr).value();
        }
    }   // namespace

    /**
     * @brief Construct a new Create Account Dialog:: Create Account Dialog
     * object
     *
     * @param availableAccounts A list of available account drafts to populate
     * the reference account combo box with, this allows the dialog to provide
     * the user with a list of existing accounts that can be used as reference
     * accounts when creating a new account, and ensures that the user can only
     * select valid reference accounts that are compatible with the new account
     * being created.
     * @param parent The parent widget of the dialog
     */
    CreateAccountDialog::CreateAccountDialog(
        const std::vector<drafts::AccountDraft>& availableAccounts,
        QWidget*                                 parent
    )
        : Dialog(parent)
    {
        for (const auto& account : availableAccounts)
        {
            switch (account.getKind())
            {
                case AccountKind::Cash:
                {
                    // we can only have one link per cash account, so we only
                    // want to show the cash accounts that are not linked to a
                    // security account in the reference account combo box, this
                    // is to prevent the user from linking multiple cash
                    // accounts to the same security account, which would be an
                    // invalid state
                    if (!account.getLinkedSecurityAccountId().has_value())
                        _availableCashAccounts.push_back(account);

                    break;
                }
                case AccountKind::Security:
                    _availableSecurityAccounts.push_back(account);
                    break;
                case AccountKind::External:
                    break;
            }
        }

        setWindowTitle("Create Account");

        _buildUI();
    }

    /**
     * @brief Build the UI of the create account dialog
     *
     */
    void CreateAccountDialog::_buildUI()
    {
        auto* mainLayout = common::makeQChild<QVBoxLayout>(this);

        _buildFormLayout(mainLayout);
        _buildButtonSection(mainLayout);
    }

    /**
     * @brief Build the form layout for creating an account
     *
     * @param parent The parent layout of the form section
     */
    void CreateAccountDialog::_buildFormLayout(QVBoxLayout* parent)
    {
        auto* formLayout = common::makeQChild<QFormLayout>();

        parent->addLayout(formLayout);

        _accountType = common::makeQChild<QComboBox>(this);
        for (const auto& type : AccountKindMeta::values)
        {
            // we disallow creating accounts of kind External, since those
            // are meant to represent accounts that exist outside of the
            // application and are not meant to be created by the user
            if (type == AccountKind::External)
                continue;

            _accountType->addItem(
                QString::fromStdString(AccountKindMeta::toString(type))
            );
        }
        formLayout->addRow("Account Type:", _accountType);
        connect(
            _accountType,
            &QComboBox::currentTextChanged,
            this,
            &CreateAccountDialog::_updateReferenceAccountVisibility
        );

        auto [nameLineEdit, nameContainer] =
            createLineEditWithLabel<NameLineEdit>(this);

        _nameLineEdit = nameLineEdit;
        _nameLineEdit->setRequired(true);
        _nameLineEdit->allowWhiteSpaces(true);

        formLayout->addRow("Account Name:", nameContainer);

        _currency = common::makeQChild<QComboBox>(this);
        for (const auto& type : CurrencyMeta::values_view())
        {
            if (type == Currency::Unknown)
                continue;

            _currency->addItem(
                QString::fromStdString(CurrencyMeta::toString(type))
            );
        }
        formLayout->addRow("Currency:", _currency);

        _referenceAccount = common::makeQChild<AccountCombo>(this);
        _referenceAccountLabel =
            common::makeQChild<QLabel>("Reference Account:");

        formLayout->addRow(_referenceAccountLabel, _referenceAccount);
        _updateReferenceAccountVisibility();
    }

    /**
     * @brief build the button section
     *
     * @param parent The parent layout of the button section
     */
    void CreateAccountDialog::_buildButtonSection(QVBoxLayout* parent)
    {
        auto* buttonLayout = common::makeQChild<QHBoxLayout>();

        _addButton = common::makeQChild<QPushButton>("Create Account", this);

        // check the validity of the input to enable or disable the add
        // button
        _addButton->setEnabled(false);
        connect(
            _nameLineEdit,
            &NameLineEdit::validityChanged,
            _addButton,
            &QPushButton::setEnabled
        );

        // connect the add button to emit the Ok action with the profile
        // draft
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &CreateAccountDialog::_emitOk
        );

        buttonLayout->addWidget(_addButton);

        _cancelButton = common::makeQChild<QPushButton>("Cancel", this);
        buttonLayout->addWidget(_cancelButton);

        parent->addLayout(buttonLayout);
    }

    /**
     * @brief Emit the requested signal with the account draft created from
     * the user input, this will be called when the add button is clicked,
     * and will take the user input from the form, create an account draft,
     * and emit the requested signal with the draft, allowing the controller
     * to handle the creation of a new account based on the user's input.
     *
     */
    void CreateAccountDialog::_emitOk()
    {
        emit requested(_getAccount(), _getSelectedReferenceAccount());
        close();
    }

    /**
     * @brief Create an account draft from the user input in the form, this
     * will take the values from the form fields and create a draft that can
     * be used by the controller to create a new account, this allows the
     * dialog to convert the raw user input into a structured format that
     * can be easily consumed by the rest of the application.
     *
     * @return drafts::AccountDraft The account draft created from the user
     * input
     */
    drafts::AccountDraft CreateAccountDialog::_getAccount() const
    {
        const auto currencyStr = _currency->currentText().toStdString();
        const auto type        = _getSelectedAccountType(_accountType);
        const auto currency    = CurrencyMeta::from_string(currencyStr).value();

        return drafts::AccountDraft{
            AccountId::invalid(),
            AccountStatus::Active,
            _nameLineEdit->text().toStdString(),
            currency,
            type,
            std::nullopt   // linkedSecurityAccountId will be set by the
                           // controller
        };
    }

    /**
     * @brief Get the selected reference account from the reference account
     * combo box, this will return an optional containing the selected
     * account draft if an account is selected, or std::nullopt if no account
     * is selected, allowing the owning dialog to easily check for a selection
     * and handle it accordingly.
     *
     * @return std::optional<AccountId> An optional containing the selected
     * account draft if an account is selected, or std::nullopt if no account
     * is selected.
     */
    std::optional<AccountId> CreateAccountDialog::_getSelectedReferenceAccount(
    ) const
    {
        if (!_referenceAccount->isVisible())
            return std::nullopt;

        const auto account = _referenceAccount->selected();

        return account.has_value() ? std::optional<AccountId>{account->getId()}
                                   : std::nullopt;
    }

    /**
     * @brief Update the visibility of the reference account combo box and
     * label based on the selected account type, this will show or hide the
     * reference account combo box and label depending on whether the selected
     * account type requires a reference account, this allows the dialog to
     * dynamically adjust the form based on the user's input, providing a more
     * intuitive and user-friendly experience.
     *
     */
    void CreateAccountDialog::_updateReferenceAccountVisibility()
    {
        const auto selectedType = _getSelectedAccountType(_accountType);

        switch (selectedType)
        {
            case AccountKind::Cash:
                _referenceAccountLabel->setText("Select a Linked Account:");
                _referenceAccount->updateAccounts(_availableSecurityAccounts);
                _referenceAccountLabel->setVisible(true);
                break;
            case AccountKind::Security:
                _referenceAccountLabel->setText("Select a reference account");
                _referenceAccount->updateAccounts(_availableCashAccounts);
                _referenceAccountLabel->setVisible(true);
                break;
            case AccountKind::External:
                // should not be possible to select this type, but if it is, we
                // will hide the reference account combo box and label, since it
                // is not applicable for external accounts
                _referenceAccount->updateAccounts({});
                _referenceAccount->setVisible(false);
                _referenceAccountLabel->setVisible(false);
                break;
        }
    }

}   // namespace ui
