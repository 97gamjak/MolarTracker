#include "finance/transaction/transactions.hpp"

#include "finance/transaction/transaction_converter.hpp"

namespace finance
{
    void Transactions::addTransactions(
        const std::vector<DomainTransaction>& transactions
    )
    {
        for (const auto& transaction : transactions)
        {
            switch (transaction.getType())
            {
                // TODO: fix this here
                case TransactionDataType::Cash:
                    _cashTransactions.push_back(
                        TransactionConverter::toCash(transaction, {}).value()
                    );
                    break;
                // TODO: make Trade to a Stock transaction type and more
                case TransactionDataType::Trade:
                    _stockTransactions.push_back(
                        TransactionConverter::toStock(transaction).value()
                    );
                    break;
            }
        }
    }
}   // namespace finance