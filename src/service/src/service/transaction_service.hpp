#ifndef __SERVICE__SRC__SERVICE__TRANSACTION_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__TRANSACTION_SERVICE_HPP__

#include <memory>

#include "service/i_transaction_service.hpp"

namespace repo
{
    class ITransactionRepo;   // Forward declaration
}   // namespace repo

namespace service
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
            const finance::DomainTransaction& transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::DomainTransaction> getTransactions(
            const idSet<AccountId>&           accountIds,
            const finance::TransactionFilter& filter
        ) override;
    };
}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__TRANSACTION_SERVICE_HPP__
