// make_transaction_widget.cpp
#include "ui/transaction/make_transaction_widget.hpp"

#include <stdexcept>

#include "ui/transaction/deposit_withdrawal_widget.hpp"
#include "ui/transaction/i_create_transaction_widget.hpp"
#include "ui/transaction/stock_widget.hpp"
#include "utils/qt_helpers.hpp"

using utils::makeQChild;

namespace ui
{

    ICreateTransactionWidget* makeTransactionWidget(
        QWidget*                          parent,
        TransactionType                   type,
        std::vector<drafts::AccountDraft> accounts
    )
    {
        return makeTransactionWidget(parent, type, std::move(accounts), {});
    }

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
                    type,
                    std::move(accounts),
                    parent
                );
            case TransactionType::Stock:
                return makeQChild<StockWidget>(
                    type,
                    std::move(accounts),
                    std::move(referenceAccounts),
                    parent
                );
        }

        throw std::logic_error("Unsupported transaction type");
    }

}   // namespace ui