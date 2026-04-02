#ifndef __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__
#define __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__

namespace finance
{
    class Transaction;
}

namespace app
{
    class ITransactionRepo
    {
       public:
        virtual ~ITransactionRepo() = default;

        /**
         * @brief Adds a transaction to the repository.
         *
         * @param transaction The transaction to add.
         */
        virtual void addTransaction(
            const finance::Transaction& transaction
        ) = 0;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__REPOS_API__I_TRANSACTION_REPO_HPP__
