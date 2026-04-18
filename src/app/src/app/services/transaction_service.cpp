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
    TransactionId TransactionService::addTransaction(
        const finance::Transaction& transaction
    )
    {
        return _transactionRepo->addTransaction(transaction);
    }

    /**
     * @brief Retrieves all transactions from the repository.
     *
     * @return std::vector<finance::Transaction> A vector of all transactions.
     */
    std::vector<finance::Transaction> TransactionService::getTransactions(
    ) const
    {
        return _transactionRepo->getTransactions();
    }

}   // namespace app
