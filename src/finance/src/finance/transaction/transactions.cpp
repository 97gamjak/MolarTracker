#include "finance/transaction/transactions.hpp"

#include "finance/account/accounts.hpp"
#include "finance/transaction/stock_transaction.hpp"
#include "finance/transaction/transaction_converter.hpp"

namespace finance
{
    void StockTransactions::sort()
    {
        std::ranges::sort(
            getItems(),
            [](const StockTransaction& txA, const StockTransaction& txB)
            { return txA.getTimestamp() < txB.getTimestamp(); }
        );
    }

    SecurityView::SecurityView(const StockTransactions& transactions)
        : _stockTransactions(transactions)
    {
    }

    void Transactions::addTransactions(
        const std::vector<DomainTransaction>& transactions,
        const Accounts&                       accounts
    )
    {
        for (const auto& transaction : transactions)
        {
            switch (transaction.getType())
            {
                // TODO: fix this here
                case TransactionDataType::Cash:
                    _cashTransactions.add(
                        TransactionConverter::toCash(transaction, accounts)
                            .value()
                    );
                    break;
                // TODO: make Trade to a Stock transaction type and more
                case TransactionDataType::Stock:
                    _stockTransactions.add(
                        TransactionConverter::toStock(transaction, accounts)
                            .value()
                    );
                    break;
            }
        }
    }

    const CashTransactions& Transactions::cash() const
    {
        return _cashTransactions;
    }

    const StockTransactions& Transactions::stocks() const
    {
        return _stockTransactions;
    }

    SecurityView Transactions::securities() const
    {
        return SecurityView(_stockTransactions);
    }

    bool Transactions::empty() const { return _getTransactions().empty(); }

    std::vector<const Transaction*> Transactions::_getTransactions() const
    {
        std::vector<const Transaction*> transactions;
        transactions.reserve(
            _cashTransactions.size() + _stockTransactions.size()
        );
        for (const auto& transaction : _cashTransactions)
        {
            transactions.push_back(
                dynamic_cast<const Transaction*>(&transaction)
            );
        }
        for (const auto& transaction : _stockTransactions)
        {
            transactions.push_back(
                dynamic_cast<const Transaction*>(&transaction)
            );
        }
        return transactions;
    }
}   // namespace finance