#include "ui/transaction/create_transaction_details.hpp"

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qlineedit.h>

#include <algorithm>

#include "finance/currency.hpp"
#include "ui/validators/amount_line_edit.hpp"
#include "ui/validators/validators.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{
    ICreateTransactionWidget::ICreateTransactionWidget(QWidget* parent)
        : QWidget(parent)
    {
    }

    NonEmptyTransactionWidget::NonEmptyTransactionWidget(
        QWidget*                          parent,
        std::vector<drafts::AccountDraft> accounts
    )
        : ICreateTransactionWidget(parent),
          _accounts(std::move(accounts)),
          _layout(new QFormLayout(this))
    {
        setLayout(_layout);
        _setAccounts();
        auto [amountField, amountContainer] =
            createLineEditWithLabel<AmountLineEdit>(this);

        _amountField = amountField;
        _amountField->setRequired(true);
        _amountField->setOnlyPositive(true);
        _amountField->setText("0");

        _currencyLabel = makeQChild<QLabel>(this);
        _currencyLabel->setText("");

        auto* amountRow = makeQChild<QHBoxLayout>(this);
        amountRow->addWidget(amountContainer);
        amountRow->addWidget(_currencyLabel);

        _layout->addRow("Amount:", amountRow);
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

    void NonEmptyTransactionWidget::_onAccountSelected(int index)
    {
        const auto accountId =
            _accountsSelection->itemData(index).value<AccountId>();

        // find the account with the selected ID
        auto it = std::ranges::find_if(
            _accounts,
            [accountId](const drafts::AccountDraft& account)
            { return account.id == accountId; }
        );

        if (it != _accounts.end())
        {
            using finance::getMicroUnit;
            using finance::getSymbol;

            _amountField->setNDecimalPlaces(getMicroUnit(it->currency));
            _currencyLabel->setText(getSymbol(it->currency).c_str());
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
                    std::move(accounts)
                );
        }
    }

    EmptyTransactionWidget::EmptyTransactionWidget(QWidget* parent)
        : ICreateTransactionWidget(parent)
    {
    }
}   // namespace ui