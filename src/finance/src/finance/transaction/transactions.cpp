#include "finance/transaction/transactions.hpp"

#include "finance/account/accounts.hpp"
#include "finance/transaction/transaction_converter.hpp"

namespace finance
{
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
                    _cashTransactions.push_back(
                        TransactionConverter::toCash(transaction, accounts)
                            .value()
                    );
                    break;
                // TODO: make Trade to a Stock transaction type and more
                case TransactionDataType::Trade:
                    _stockTransactions.push_back(
                        TransactionConverter::toStock(transaction, accounts)
                            .value()
                    );
                    break;
            }
        }
    }

    const std::vector<CashTransaction>& Transactions::getCashTransactions(
    ) const
    {
        return _cashTransactions;
    }

    const std::vector<StockTransaction>& Transactions::getStockTransactions(
    ) const
    {
        return _stockTransactions;
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