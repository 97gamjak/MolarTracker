#include "ui/transaction/create_transaction_details.hpp"

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>

#include <algorithm>

#include "config/finance.hpp"
#include "finance/currency.hpp"
#include "ui/validators/amount_line_edit.hpp"
#include "ui/validators/validators.hpp"
#include "utils/qt_helpers.hpp"
#include "utils/timestamp.hpp"

using utils::makeQChild;

namespace ui
{
    ICreateTransactionWidget::ICreateTransactionWidget(QWidget* parent)
        : QWidget(parent)
    {
    }

    NonEmptyTransactionWidget::NonEmptyTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
        : ICreateTransactionWidget(parent),
          _accounts(std::move(accounts)),
          _type(type),
          _layout(new QFormLayout(this)),
          _currencyLabel(new QLabel(this)),
          _addButton(new QPushButton("Add Transaction"))
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
        _addButton->setEnabled(_amountField->isValid());
        connect(
            _amountField,
            &AmountLineEdit::validityChanged,
            _addButton,
            &QPushButton::setEnabled
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

    TransactionType NonEmptyTransactionWidget::_getTransactionType() const
    {
        return _type;
    }

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

    micro_units NonEmptyTransactionWidget::_getAmount() const
    {
        return _amountField->text().toLongLong();
    }

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

    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
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
        }
    }

    EmptyTransactionWidget::EmptyTransactionWidget(QWidget* parent)
        : ICreateTransactionWidget(parent)
    {
    }

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

        const auto amount = _getTransactionType() == TransactionType::Deposit
                                ? _getAmount()
                                : -_getAmount();

        auto entry = drafts::TransactionEntryDraft{
            selectedAccount->id,
            finance::Cash(selectedAccount->currency, amount),
            selectedAccount->kind
        };
        entry.needsExternal = true;

        draft.entries.push_back(entry);

        return draft;
    }

    void NonEmptyTransactionWidget::_emitOk()
    {
        const auto draft = getDraft();
        emit       createTransactionRequested(draft);
    }

}   // namespace ui