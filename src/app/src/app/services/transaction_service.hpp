#ifndef __APP__SRC__APP__SERVICES__TRANSACTION_SERVICE_HPP__
#define __APP__SRC__APP__SERVICES__TRANSACTION_SERVICE_HPP__

#include <memory>

#include "app/services_api/i_transaction_service.hpp"

namespace repo
{
    class ITransactionRepo;   // Forward declaration
}   // namespace repo

namespace app
{

    /**
     * @brief Service for managing transactions.
     *
     */
    class TransactionService : public ITransactionService
    {
       private:
        /// Pointer to the transaction repository.
        std::shared_ptr<repo::ITransactionRepo> _transactionRepo;

       public:
        explicit TransactionService(
            std::shared_ptr<repo::ITransactionRepo> transactionRepo
        );

        [[nodiscard]]
        TransactionId addTransaction(
            const finance::Transaction& transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions(
            const idSet<AccountId>&           accountIds,
            const finance::TransactionFilter& filter
        ) override;
    };
}   // namespace app

#endif   // __APP__SRC__APP__SERVICES__TRANSACTION_SERVICE_HPP__
