#include "ui/transaction/deposit_withdrawal_widget.hpp"

#include <qboxlayout.h>
#include <qdialog.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <stdexcept>

#include "config/finance.hpp"
#include "drafts/transaction/transaction_create_draft.hpp"
#include "finance/currency.hpp"
#include "ui/transaction/account_combo.hpp"
#include "ui/transaction/amount_row.hpp"
#include "ui/transaction/comment_field.hpp"
#include "ui/transaction/timestamp_field.hpp"
#include "ui/utils/error.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    /**
     * @brief Struct for holding the fields in the deposit/withdrawal widget
     *
     */
    struct DepositWithdrawalWidget::Fields
    {
        /// The combo box for selecting the account to deposit to or withdraw
        /// from
        AccountCombo* accountCombo;

        /// The field for selecting the transaction timestamp
        TimestampField* timestampField;

        /// The row for entering the amount to deposit or withdraw
        AmountRow* amountRow;

        /// The label for displaying the currency of the selected account
        QLabel* currencyLabel;

        /// The field for entering an optional comment
        CommentField* commentField;

        /// The row for entering the fees for the transaction
        AmountRow* feesRow;

        /// The label for displaying the currency of the fees
        QLabel* currencyFees;

        Fields(
            const std::vector<drafts::AccountDraft>& accounts,
            QWidget*                                 parent
        );

        void addFieldsToLayout(QFormLayout* layout) const;
        void updateCurrency(Currency currency) const;
        void updateFields() const;

        [[nodiscard]]
        drafts::CreateCashTransactionDraft getDraft(TransactionType type) const;
    };

    /**
     * @brief Construct a new Deposit Withdrawal Widget:: Fields:: Fields object
     *
     * @param accounts
     * @param parent
     */
    DepositWithdrawalWidget::Fields::Fields(
        const std::vector<drafts::AccountDraft>& accounts,
        QWidget*                                 parent
    )
        : accountCombo(makeQChild<AccountCombo>(accounts, parent)),
          timestampField(makeQChild<TimestampField>(parent)),
          amountRow(makeQChild<AmountRow>(parent)),
          currencyLabel(makeQChild<QLabel>(parent)),
          commentField(makeQChild<CommentField>(parent)),
          feesRow(makeQChild<AmountRow>(parent)),
          currencyFees(makeQChild<QLabel>(parent))
    {
    }

    /**
     * @brief Update the fields in the deposit/withdrawal widget
     *
     */
    void DepositWithdrawalWidget::Fields::updateFields() const
    {
        accountCombo->update();
        timestampField->update();
        amountRow->update();
        currencyLabel->update();
        commentField->update();
        feesRow->update();
    }

    /**
     * @brief Update the currency labels and decimal places for the amount and
     * fees rows
     *
     * @param currency The currency to update the fields for
     */
    void DepositWithdrawalWidget::Fields::updateCurrency(
        Currency currency
    ) const
    {
        using finance::getMicroUnit;
        using finance::getSymbol;

        amountRow->setNDecimalPlaces(getMicroUnit(currency));
        feesRow->setNDecimalPlaces(getMicroUnit(currency));
        currencyLabel->setText(getSymbol(currency).c_str());
        currencyFees->setText(getSymbol(currency).c_str());
    }

    /**
     * @brief Add the fields to the layout
     *
     * @param layout The layout to add the fields to
     */
    void DepositWithdrawalWidget::Fields::addFieldsToLayout(
        QFormLayout* layout
    ) const
    {
        layout->addRow("Account:", accountCombo);
        layout->addRow("Timestamp:", timestampField);

        auto* amountRowLayout = makeQChild<QHBoxLayout>();
        amountRowLayout->addWidget(amountRow);
        amountRowLayout->addWidget(currencyLabel);
        layout->addRow("Amount:", amountRowLayout);

        auto* feesRowLayout = makeQChild<QHBoxLayout>();
        feesRowLayout->addWidget(feesRow);
        feesRowLayout->addWidget(currencyFees);
        layout->addRow("Fees:", feesRowLayout);

        layout->addRow("Comment:", commentField);
    }

    /**
     * @brief Get the transaction draft based on the current input in the
     * widget, this will generate a CreateCashTransactionDraft based on the
     * selected account and entered amount, which can then be used by the owning
     * dialog to create a new cash transaction in the store when the user
     * submits the form.
     *
     * @note The amount in the generated draft will be negative for Withdrawal
     * transactions and positive for Deposit transactions, reflecting the
     * direction of the cash flow for each transaction type.
     *
     * @param type The type of transaction (Deposit or Withdrawal)
     *
     * @return drafts::CreateCashTransactionDraft The transaction draft
     * generated from the current input in the widget, which can be used to
     * create a new cash transaction in the store.
     */
    drafts::CreateCashTransactionDraft DepositWithdrawalWidget::Fields::
        getDraft(TransactionType type) const
    {
        const auto account = accountCombo->selected();

        if (!account.has_value())
            throw std::runtime_error("No account selected");

        const auto currency   = account->getCurrency();
        const auto microUnits = finance::getMicroUnit(currency);
        const auto cash_      = amountRow->getAmount(microUnits);

        auto cash = finance::Cash(currency, cash_);
        cash      = type == TransactionType::Deposit ? cash : -cash;

        auto fees = finance::Cash(currency, feesRow->getAmount(microUnits));

        return drafts::CreateCashTransactionDraft{
            timestampField->getTimestamp(),
            cash,
            fees,
            account->getId(),
            commentField->getComment()
        };
    }

    /**
     * @brief Construct a new Deposit Withdrawal Widget:: Deposit Withdrawal
     * Widget object
     *
     * @param type The type of transaction (Deposit or Withdrawal)
     * @param accounts A list of account drafts to populate the account combo
     * box with
     * @param parent The parent widget for this widget
     */
    DepositWithdrawalWidget::DepositWithdrawalWidget(
        TransactionType                          type,
        const std::vector<drafts::AccountDraft>& accounts,
        QWidget*                                 parent
    )
        : Dialog(parent),
          _type(type),
          _layout(new QFormLayout(this)),
          _addButton(makeQChild<QPushButton>("Add Transaction", this)),
          _fields(std::make_unique<Fields>(accounts, this))
    {
        setLayout(_layout);

        _fields->addFieldsToLayout(_layout);

        _addButton->setEnabled(false);
        auto* buttonLayout = makeQChild<QHBoxLayout>();
        buttonLayout->addWidget(_addButton);
        _layout->addRow(buttonLayout);

        connect(
            _fields->accountCombo,
            &AccountCombo::accountSelected,
            this,
            &DepositWithdrawalWidget::_onAccountSelected
        );
        connect(
            _fields->amountRow,
            &AmountRow::validityChanged,
            this,
            &DepositWithdrawalWidget::_updateAddButton
        );
        connect(
            _fields->amountRow,
            &AmountRow::valueChanged,
            this,
            &DepositWithdrawalWidget::_updateAddButton
        );
        connect(
            _addButton,
            &QPushButton::clicked,
            this,
            &DepositWithdrawalWidget::_emitOk
        );
    }

    DepositWithdrawalWidget::~DepositWithdrawalWidget() = default;

    /**
     * @brief Update the list of accounts in the account combo box
     *
     * @param accounts The new list of account drafts to populate the combo box
     */
    void DepositWithdrawalWidget::updateAccounts(
        std::vector<drafts::AccountDraft> accounts
    )
    {
        _fields->accountCombo->updateAccounts(std::move(accounts));
    }

    /**
     * @brief Refresh the widget to reflect the current state
     *
     */
    void DepositWithdrawalWidget::refresh()
    {
        _fields->updateFields();
        _addButton->update();
    }

    /**
     * @brief Get the transaction type of this widget, this will return the type
     * of transaction that this widget is designed to create (either Deposit or
     * Withdrawal), allowing the owning dialog to determine how to handle the
     * transaction draft generated by this widget.
     *
     * @return TransactionType The transaction type of this widget
     */
    TransactionType DepositWithdrawalWidget::getTransactionType() const
    {
        return _type;
    }

    /**
     * @brief Set the transaction type of this widget
     *
     * @param type The transaction type to set
     */
    void DepositWithdrawalWidget::setTransactionType(TransactionType type)
    {
        _type = type;
    }

    /**
     * @brief Get the transaction draft based on the current input in the
     * widget, this will generate a CreateCashTransactionDraft based on the
     * selected account and entered amount, which can then be used by the owning
     * dialog to create a new cash transaction in the store when the user
     * submits the form.
     *
     * @note The amount in the generated draft will be negative for Withdrawal
     * transactions and positive for Deposit transactions, reflecting the
     * direction of the cash flow for each transaction type.
     * @return drafts::CreateCashTransactionDraft The transaction draft
     * generated from the current input in the widget, which can be used to
     * create a new cash transaction in the store.
     */
    drafts::CreateCashTransactionDraft DepositWithdrawalWidget::_getDraft(
    ) const
    {
        return _fields->getDraft(_type);
    }

    /**
     * @brief Handle the selection of an account in the account combo box, this
     * will update the amount row to have the correct number of decimal places
     * for the selected account's currency, and will also update the currency
     * label to show the symbol of the selected account's currency, providing
     * feedback to the user about the currency they are entering an amount for,
     * and ensuring that the amount is properly formatted for the selected
     * account.
     *
     * @param account The selected account draft from the combo box, this is
     * used to determine the currency and formatting for the amount row and
     * currency label.
     */
    void DepositWithdrawalWidget::_onAccountSelected(
        const drafts::AccountDraft& account
    )
    {
        _fields->updateCurrency(account.getCurrency());
        _updateAddButton();
    }

    /**
     * @brief Update the enabled state of the add button based on the current
     * input in the widget, this will enable the add button only when a valid
     * account is selected and a valid, non-zero amount is entered, providing
     * feedback to the user about when they can submit the form to create a new
     * transaction, and preventing invalid transactions from being created.
     *
     * The add button will be enabled only when:
     * - An account is selected in the account combo box
     * - The amount entered in the amount row is valid (e.g. properly formatted)
     * - The amount entered in the amount row is not zero (since a zero amount
     *   would not make sense for a deposit or withdrawal transaction)
     */
    void DepositWithdrawalWidget::_updateAddButton()
    {
        const auto isValid = _fields->accountCombo->selected().has_value() &&
                             _fields->amountRow->isValid() &&
                             !_fields->amountRow->isZero();

        _addButton->setEnabled(isValid);
    }

    /**
     * @brief Emit the signal to create a new transaction, this will be called
     * when the user clicks the add button, and will emit the
     * createTransactionRequested signal with the transaction type of this
     * widget, allowing the owning dialog to handle the creation of the new
     * transaction based on the current input in the widget.
     *
     * The owning dialog can then call getDraft() on this widget to retrieve the
     * transaction draft based on the current input, and use that draft to
     * create a new transaction in the store. This allows the widget to
     * communicate with the dialog and controller to facilitate the creation of
     * new transactions based on user input, while keeping the widget focused on
     * managing the input and validation for the transaction details.
     */
    void DepositWithdrawalWidget::_emitOk()
    {
        try
        {
            const auto draft = _getDraft();
            emit       createCashTransactionRequested(draft);
        }
        catch (const std::exception& e)
        {
            ErrorDialog::show(QString(e.what()));
        }
    }

}   // namespace ui