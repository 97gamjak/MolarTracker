#ifndef __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Transaction;
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
         * @return A vector of all transactions.
         */
        [[nodiscard]] virtual std::vector<finance::Transaction> getTransactions(
        ) = 0;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__
