#include "ui/transaction/create_transaction_details.hpp"

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

#include <algorithm>

#include "config/finance.hpp"
#include "drafts/transaction_draft.hpp"
#include "exceptions/not_yet_implemented.hpp"
#include "finance/currency.hpp"
#include "ui/validators/amount_line_edit.hpp"
#include "ui/validators/validators.hpp"
#include "utils/qt_helpers.hpp"
#include "utils/timestamp.hpp"

using utils::makeQChild;

namespace ui
{
    /**
     * @brief Construct a new ICreateTransactionWidget::ICreateTransactionWidget
     * object
     *
     * @param parent
     */
    ICreateTransactionWidget::ICreateTransactionWidget(QWidget* parent)
        : QWidget(parent)
    {
    }

    /**
     * @brief Construct a new Non Empty Transaction Widget:: Non Empty
     * Transaction Widget object
     *
     * @param parent
     * @param type
     * @param accounts
     */
    NonEmptyTransactionWidget::NonEmptyTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
        : ICreateTransactionWidget(parent),
          _accounts(std::move(accounts)),
          _type(type),
          _currencyLabel(new QLabel(this)),
          _addButton(new QPushButton("Add Transaction")),
          _layout(new QFormLayout(this))
    {
        _layout->setObjectName("transactionDetailsLayout");
        setLayout(_layout);
        _setAccounts();
        auto [amountField, amountContainer] =
            createLineEditWithLabel<AmountLineEdit>(this);

        _amountField = amountField;
        _amountField->setRequired(true);
        _amountField->setOnlyPositive(true);
        _amountField->setText("0");

        _currencyLabel->setText("");

        auto* amountRow = makeQChild<QHBoxLayout>();
        amountRow->addWidget(amountContainer);
        amountRow->addWidget(_currencyLabel);

        _layout->addRow("Amount:", amountRow);

        auto* buttonLayout = utils::makeQChild<QHBoxLayout>();

        // check the validity of the input to enable or disable the add button
        _addButton->setEnabled(false);
        connect(
            _amountField,
            &AmountLineEdit::validityChanged,
            this,
            &NonEmptyTransactionWidget::_enableAddButtonIfValid
        );
        connect(
            _amountField,
            &AmountLineEdit::valueChanged,
            this,
            &NonEmptyTransactionWidget::_enableAddButtonIfValid
        );

        // connect the add button to emit the Ok action with the profile draft
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &NonEmptyTransactionWidget::_emitOk
        );

        buttonLayout->addWidget(_addButton);

        _layout->addRow(buttonLayout);
    }

    /**
     * @brief get the transaction type for this widget
     *
     * @return TransactionType
     */
    TransactionType NonEmptyTransactionWidget::_getTransactionType() const
    {
        return _type;
    }

    /**
     * @brief get the selected account from the accounts selection combo box,
     * this will return an optional containing the selected account draft if an
     * account is selected, or an empty optional if no account is selected,
     * allowing the caller to handle the case where no account is selected when
     * trying to get the details for creating a transaction.
     *
     * @return std::optional<drafts::AccountDraft>
     */
    std::optional<drafts::AccountDraft> NonEmptyTransactionWidget::
        _getSelectedAccount() const
    {
        const auto accountId =
            _accountsSelection->currentData().value<AccountId>();

        auto it = std::ranges::find_if(
            _accounts,
            [accountId](const drafts::AccountDraft& account)
            { return account.id == accountId; }
        );

        if (it != _accounts.end())
        {
            return *it;
        }

        return std::nullopt;
    }

    /**
     * @brief set the accounts for the accounts selection combo box
     */
    void NonEmptyTransactionWidget::_setAccounts()
    {
        _accountsSelection = makeQChild<QComboBox>(this);
        _accountsSelection->setPlaceholderText("Select Account");
        _layout->addRow("Account:", _accountsSelection);

        for (const auto& account : _accounts)
        {
            _accountsSelection->addItem(
                QString::fromStdString(account.name),
                QVariant::fromValue(account.id)
            );
        }

        connect(
            _accountsSelection,
            &QComboBox::activated,
            this,
            &NonEmptyTransactionWidget::_onAccountSelected
        );
    }

    /**
     * @brief Construct a new Security Widget:: Security Widget object
     *
     * @param parent
     * @param type
     * @param accounts
     * @param referenceAccounts
     */
    SecurityWidget::SecurityWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts,
        std::vector<drafts::AccountDraft> referenceAccounts
    )
        : NonEmptyTransactionWidget(parent, type, std::move(accounts)),
          _referenceAccounts(std::move(referenceAccounts))
    {
    }

    /**
     * @brief Handle the selection of an account from the accounts combo box,
     * this will be called when the user selects an account from the combo box,
     * and should handle updating any relevant UI elements (e.g. currency label)
     * based on the selected account, allowing the user to see the appropriate
     * information for the selected account when creating a transaction.
     *
     * @param index The index of the selected account in the combo box, this can
     * be used to retrieve the corresponding AccountDraft from the _accounts
     * vector based on the data stored in the combo box items, allowing the
     * method to access the details of the selected account and update the UI
     * accordingly.
     */
    void NonEmptyTransactionWidget::_onAccountSelected(int /*index*/)
    {
        const auto account = _getSelectedAccount();

        if (account.has_value())
        {
            using finance::getMicroUnit;
            using finance::getSymbol;

            _amountField->setNDecimalPlaces(getMicroUnit(account->currency));
            _currencyLabel->setText(getSymbol(account->currency).c_str());
        }
    }

    /**
     * @brief factory function to create a transaction widget based on the
     * transaction type
     *
     * @param parent The parent widget
     * @param type The type of the transaction
     * @param accounts The list of account drafts to choose from
     *
     * @return ICreateTransactionWidget* A pointer to the created transaction
     */
    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
    {
        return makeTransactionWidget(parent, type, std::move(accounts), {});
    }

    /**
     * @brief factory function to create a transaction widget based on the
     * transaction type
     *
     * @param parent The parent widget
     * @param type The type of the transaction
     * @param accounts The list of account drafts to choose from
     *
     * @return ICreateTransactionWidget* A pointer to the created transaction
     */
    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts,
        std::vector<drafts::AccountDraft> referenceAccounts
    )
    {
        switch (type)
        {
            case TransactionType::Deposit:
            case TransactionType::Withdrawal:
                return makeQChild<DepositWithdrawalWidget>(
                    parent,
                    type,
                    std::move(accounts)
                );
            case TransactionType::Stock:
                return makeQChild<StockWidget>(
                    parent,
                    type,
                    std::move(accounts),
                    std::move(referenceAccounts)
                );
        }

        throw std::invalid_argument("Unsupported transaction type");
    }

    /**
     * @brief Construct a new Empty Transaction Widget:: Empty Transaction
     * Widget object
     *
     * @param parent
     */
    EmptyTransactionWidget::EmptyTransactionWidget(QWidget* parent)
        : ICreateTransactionWidget(parent)
    {
    }

    /**
     * @brief get the transaction draft from the empty transaction widget
     *
     * @return drafts::TransactionDraft
     */
    drafts::TransactionDraft DepositWithdrawalWidget::getDraft() const
    {
        drafts::TransactionDraft draft;
        Timestamp                now = Timestamp{};

        draft.timestamp = now;

        const auto selectedAccount = _getSelectedAccount();

        if (!selectedAccount.has_value())
        {
            throw std::runtime_error("No account selected");
        }

        const auto amountTmp = _getAmountField()->getAmount();
        const auto amount    = _getTransactionType() == TransactionType::Deposit
                                   ? amountTmp
                                   : -amountTmp;

        auto entry = drafts::TransactionEntryDraft{
            selectedAccount->id,
            finance::Cash(selectedAccount->currency, amount),
            selectedAccount->kind
        };
        entry.needsExternal = true;

        draft.entries.push_back(entry);

        return draft;
    }

    drafts::TransactionDraft StockWidget::getDraft() const
    {
        // TODO (97gamjak)[MOLTRACK-233]: implement StockWidget::getDraft()
        throw NotYetImplementedException(
            "StockWidget::getDraft() not yet implemented"
        );
    }

    /**
     * @brief Emit the createTransactionRequested signal with the transaction
     * draft
     *
     */
    void NonEmptyTransactionWidget::_emitOk()
    {
        const auto draft = getDraft();
        emit       createTransactionRequested(draft);
    }

    /**
     * @brief Enable or disable the add button based on the validity of the
     * input, this will check if the selected account is valid and if the amount
     * entered is valid and not zero, and will enable the add button if the
     * input is valid, or disable it if the input is invalid, providing feedback
     * to the user about whether they can submit the form to create a
     * transaction draft based on the current input values.
     *
     */
    void NonEmptyTransactionWidget::_enableAddButtonIfValid()
    {
        const auto isValid = _getSelectedAccount().has_value() &&
                             _amountField->isValid() &&
                             _amountField->getAmount() != 0;

        _addButton->setEnabled(isValid);
    }

    /**
     * @brief get the amount field
     *
     * @return AmountLineEdit*
     */
    AmountLineEdit* NonEmptyTransactionWidget::_getAmountField() const
    {
        return _amountField;
    }

}   // namespace ui