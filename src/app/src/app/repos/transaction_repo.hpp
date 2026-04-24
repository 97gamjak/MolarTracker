#ifndef __APP__SRC__APP__REPOS__TRANSACTION_REPO_HPP__
#define __APP__SRC__APP__REPOS__TRANSACTION_REPO_HPP__

#include <memory>

#include "app/repos_api/i_transaction_repo.hpp"
#include "base_repo.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"

namespace app
{
    class IInstrumentRepo;

    /**
     * @brief Repository for managing transactions.
     *
     */
    class TransactionRepo : public ITransactionRepo, public BaseRepo
    {
       private:
        std::shared_ptr<IInstrumentRepo> _instrumentRepo;

       public:
        explicit TransactionRepo(
            db::Database&                    db,
            std::shared_ptr<IInstrumentRepo> instrumentRepo
        );

        [[nodiscard]]
        TransactionId addTransaction(
            const finance::Transaction& transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions() override;
    };
}   // namespace app

#endif   // __APP__SRC__APP__REPOS__TRANSACTION_REPO_HPP__
