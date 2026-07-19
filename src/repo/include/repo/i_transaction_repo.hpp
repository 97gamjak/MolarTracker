#ifndef __REPO__INCLUDE__REPO__I_TRANSACTION_REPO_HPP__
#define __REPO__INCLUDE__REPO__I_TRANSACTION_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    class DomainTransaction;    // Forward declaration
    struct TransactionFilter;   // Forward declaration
}   // namespace finance

namespace repo
{
    /**
     * @brief Interface for transaction repository
     *
     */
    class ITransactionRepo
    {
       public:
        virtual ~ITransactionRepo() = default;

        /**
         * @brief Adds a transaction to the repository.
         *
         * @param transaction The transaction to add.
         *
         * @return The added transaction.
         */
        [[nodiscard]]
        virtual TransactionId addTransaction(
            const finance::DomainTransaction& transaction
        ) = 0;

        /**
         * @brief Retrieves all transactions from the repository.
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
}   // namespace repo

#endif   // __REPO__INCLUDE__REPO__I_TRANSACTION_REPO_HPP__
