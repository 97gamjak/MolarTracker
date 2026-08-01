#ifndef __SERVICE__INCLUDE__SERVICE__I_TRANSACTION_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_TRANSACTION_SERVICE_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "error/crud_error.hpp"

namespace finance
{
    class DomainTransaction;    // Forward declaration
    struct TransactionFilter;   // Forward declaration
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
         * @return CrudResult<TransactionId> The result of the add operation.
         */
        [[nodiscard]]
        virtual CrudResult<TransactionId> addTransaction(
            const finance::DomainTransaction& transaction
        ) = 0;

        /**
         * @brief Retrieves all transactions from the service.
         *
         * @param filter The filter to apply to the transactions, this will be
         * converted to a WhereExpr and applied to the query when fetching
         * transactions from the database, if no filter is provided all
         * transactions will be returned
         *
         * @return A vector of all transactions.
         */
        [[nodiscard]]
        virtual std::vector<finance::DomainTransaction> getTransactions(
            const finance::TransactionFilter& filter
        ) = 0;
    };
}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_TRANSACTION_SERVICE_HPP__
