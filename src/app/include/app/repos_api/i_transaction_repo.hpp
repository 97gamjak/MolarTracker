#ifndef __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Transaction;         // Forward declaration
    class TransactionFilter;   // Forward declaration
}   // namespace finance

namespace app
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
        [[nodiscard]] virtual TransactionId addTransaction(
            const finance::Transaction& transaction
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
        [[nodiscard]] virtual std::vector<finance::Transaction> getTransactions(
            const finance::TransactionFilter& filter
        ) = 0;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__
