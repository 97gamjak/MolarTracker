#include "ui/transaction/create_transaction_details.hpp"

#include <qboxlayout.h>
#include <qcombobox.h>
#include <qlabel.h>

#include <algorithm>

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{
    CreateTransactionWidget::CreateTransactionWidget(
        QWidget*                          parent,
        std::vector<drafts::AccountDraft> accounts
    )
        : QWidget(parent),
          _accounts(std::move(accounts)),
          _layout(new QVBoxLayout(this))
    {
        setLayout(_layout);
        _currencyLabel = makeQChild<QLabel>(this);
        _setAccounts();
    }

    void CreateTransactionWidget::_setAccounts()
    {
        if (_accounts.empty())
            return;

        _accountsSelection = makeQChild<QComboBox>(this);
        _accountsSelection->setPlaceholderText("Select Account");
        _layout->addWidget(_accountsSelection);

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
            &CreateTransactionWidget::_onAccountSelected
        );

        _currencyLabel = makeQChild<QLabel>(this);
        _layout->addWidget(_currencyLabel);

        _onAccountSelected(_accountsSelection->currentIndex());
    }

    void CreateTransactionWidget::_onAccountSelected(int index)
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
            _currencyLabel->setText(
                QString::fromStdString(CurrencyMeta::toString(it->currency))
            );
        }
    }

    CreateTransactionWidget* makeTransactionWidget(
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
        : CreateTransactionWidget(parent, {})
    {
    }
}   // namespace ui