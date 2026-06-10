#ifndef __SERVICE__INCLUDE__SERVICE__I_TRANSACTION_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_TRANSACTION_SERVICE_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class DomainTransaction;   // Forward declaration
    class TransactionFilter;   // Forward declaration
}   // namespace finance

namespace service
{
    /**
     * @brief Interface for transaction service
     *
     */
    class ITransactionService
    {
       public:
        virtual ~ITransactionService() = default;

        /**
         * @brief Adds a transaction to the service.
         *
         * @param transaction The transaction to add.
         *
         * @return The ID of the added transaction.
         */
        [[nodiscard]]
        virtual TransactionId addTransaction(
            const finance::DomainTransaction& transaction
        ) = 0;

        /**
         * @brief Retrieves all transactions from the service.
         *
         * @param accountIds The IDs of the accounts to retrieve transactions
         * for.
         * @param filter The filter to apply to the transactions, this will be
         * converted to a WhereExpr and applied to the query when fetching
         * transactions from the database, if no filter is provided all
         * transactions will be returned
         *
         * @return A vector of all transactions.
         */
        [[nodiscard]]
        virtual std::vector<finance::DomainTransaction> getTransactions(
            const idSet<AccountId>&           accountIds,
            const finance::TransactionFilter& filter
        ) = 0;
    };
}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_TRANSACTION_SERVICE_HPP__