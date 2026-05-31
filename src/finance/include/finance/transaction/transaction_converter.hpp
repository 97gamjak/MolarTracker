#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__

#include <expected>

#include "finance/account/accounts.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace finance
{
    struct TransactionConversionError
    {
        std::string message;
    };

    class TransactionConverter
    {
       public:
        [[nodiscard]]
        static DomainTransaction toDomain(const CashTransaction& transaction);

        [[nodiscard]]
        static DomainTransaction toDomain(const StockTransaction& transaction);

        [[nodiscard]]
        static std::
            expected<CashTransaction, TransactionConversionError> toCash(
                const DomainTransaction& transaction,
                const Accounts&          accounts
            );

        [[nodiscard]]
        static std::
            expected<StockTransaction, TransactionConversionError> toStock(
                const DomainTransaction& transaction
            );
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__