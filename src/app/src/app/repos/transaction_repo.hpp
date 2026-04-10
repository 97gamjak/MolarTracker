#ifndef __APP__SRC__APP__REPOS__TRANSACTION_REPO_HPP__
#define __APP__SRC__APP__REPOS__TRANSACTION_REPO_HPP__

#include "app/repos_api/i_transaction_repo.hpp"
#include "base_repo.hpp"
#include "config/id_types.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
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
            const finance::Transaction& transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions() override;

       private:
        [[nodiscard]]
        std::optional<InstrumentId> _getInstrument(const InstrumentRow& row);

        [[nodiscard]]
        InstrumentId _insertInstrument(const InstrumentRow& row);
    };
}   // namespace app

#endif   // __APP__SRC__APP__REPOS__TRANSACTION_REPO_HPP__
