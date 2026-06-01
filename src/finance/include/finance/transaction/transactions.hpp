#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__

#include <vector>

#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace finance
{
    class Accounts;

    class Transactions
    {
       private:
        std::vector<CashTransaction>  _cashTransactions;
        std::vector<StockTransaction> _stockTransactions;

       public:
        void addTransactions(
            const std::vector<DomainTransaction>& transactions,
            const Accounts&                       accounts
        );

        [[nodiscard]]
        const std::vector<CashTransaction>& getCashTransactions() const;

        [[nodiscard]]
        const std::vector<StockTransaction>& getStockTransactions() const;

        [[nodiscard]] bool empty() const;

       private:
        [[nodiscard]] std::vector<const Transaction*> _getTransactions() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTIONS_HPP__
