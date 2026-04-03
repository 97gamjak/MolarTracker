#include "transaction_service.hpp"

namespace app
{
    /**
     * @brief Constructs a TransactionService.
     *
     * @param transactionRepo The transaction repository to use.
     */
    TransactionService::TransactionService(
        std::shared_ptr<ITransactionRepo> transactionRepo
    )
        : _transactionRepo(std::move(transactionRepo))
    {
    }

    /**
     * @brief Adds a transaction to the repository.
     *
     * @param transaction The transaction to add.
     * @return TransactionId The ID of the added transaction.
     */
    [[nodiscard]] TransactionId TransactionService::addTransaction(
        const finance::Transaction& transaction
    )
    {
        return _transactionRepo->addTransaction(transaction);
    }
}   // namespace app
