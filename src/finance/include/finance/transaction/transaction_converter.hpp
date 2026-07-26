#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__

#include "error/finance_error.hpp"
#include "finance/account/accounts.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace finance
{
    /**
     * @brief Converter class for transforming between domain transactions and
     * specific transaction types (cash and stock).
     *
     */
    class TransactionConverter
    {
       public:
        [[nodiscard]]
        static DomainTransaction toDomain(
            const CashTransaction& transaction,
            const Accounts&        accounts
        );

        [[nodiscard]]
        static DomainTransaction toDomain(
            const StockTransaction& transaction,
            const Accounts&         accounts
        );

        [[nodiscard]]
        static DomainTransaction toDomain(
            const OptionTransaction& transaction,
            const Accounts&          accounts
        );

        [[nodiscard]]
        static FinanceResult<CashTransaction> toCash(
            const DomainTransaction& transaction,
            const Accounts&          accounts
        );

        [[nodiscard]]
        static FinanceResult<StockTransaction> toStock(
            const DomainTransaction& transaction
        );

        [[nodiscard]]
        static FinanceResult<OptionTransaction> toOption(
            const DomainTransaction& transaction
        );
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__