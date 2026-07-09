#ifndef __REPO__SRC__REPO__TRANSACTION_REPO_HPP__
#define __REPO__SRC__REPO__TRANSACTION_REPO_HPP__

#include "base_repo.hpp"
#include "config/id_types.hpp"
#include "repo/i_transaction_repo.hpp"

namespace repo
{
    /**
     * @brief Repository for managing transactions.
     *
     */
    class TransactionRepo : public ITransactionRepo, public BaseRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]]
        TransactionId addTransaction(
            const finance::DomainTransaction& transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::DomainTransaction> getTransactions(
            const idSet<AccountId>&           accountIds,
            const finance::TransactionFilter& filter
        ) override;
    };
}   // namespace repo

#endif   // __REPO__SRC__REPO__TRANSACTION_REPO_HPP__
