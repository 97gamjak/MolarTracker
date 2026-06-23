#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__

#include <expected>

#include "finance/account/accounts.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/option_transaction.hpp"
#include "finance/transaction/stock_transaction.hpp"

namespace finance
{
    /**
     * @brief Structure representing an error that occurs during transaction
     * conversion.
     *
     */
    struct TransactionConversionError
    {
        /// A message describing the error that occurred during transaction
        /// conversion.
        std::string message;
    };

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

        [[nodiscard]]
        static std::
            expected<OptionTransactionTemporary, TransactionConversionError> toOption(
                const DomainTransaction& transaction
            );
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_CONVERTER_HPP__