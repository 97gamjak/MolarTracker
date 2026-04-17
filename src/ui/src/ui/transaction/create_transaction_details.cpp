#include "ui/transaction/create_transaction_details.hpp"

#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{
    CreateTransactionWidget::CreateTransactionWidget(QWidget* parent)
        : QWidget(parent)
    {
    }

    void CreateTransactionWidget::setAccounts(
        std::vector<drafts::AccountDraft> accounts
    )
    {
        _accounts = std::move(accounts);
    }

    std::optional<TransactionType> CreateTransactionWidget::getTransactionType(
    ) const
    {
        return _transactionType;
    }

    CreateTransactionWidget* makeTransactionWidget(
        QWidget*                       parent,
        std::optional<TransactionType> type
    )
    {
        if (!type.has_value())
            return makeQChild<EmptyTransactionWidget>(parent);

        switch (type.value())
        {
            case TransactionType::Deposit:
            case TransactionType::Withdrawal:
                return makeQChild<DepositWithdrawalWidget>(parent);
        }
    }
}   // namespace ui