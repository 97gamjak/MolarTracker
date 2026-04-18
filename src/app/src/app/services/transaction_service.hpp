#ifndef __APP__SRC__APP__SERVICES__TRANSACTION_SERVICE_HPP__
#define __APP__SRC__APP__SERVICES__TRANSACTION_SERVICE_HPP__

#include "app/repos_api/i_transaction_repo.hpp"
#include "app/services_api/i_transaction_service.hpp"
#include "finance/transaction.hpp"

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
        std::shared_ptr<ITransactionRepo> _transactionRepo;

       public:
        explicit TransactionService(
            std::shared_ptr<ITransactionRepo> transactionRepo
        );

        [[nodiscard]]
        TransactionId addTransaction(
            const finance::Transaction& transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions() const override;
    };
}   // namespace app

#endif   // __APP__SRC__APP__SERVICES__TRANSACTION_SERVICE_HPP__
