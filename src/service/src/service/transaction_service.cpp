#include "transaction_service.hpp"

#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "repo/i_transaction_repo.hpp"

namespace service
{
    /**
     * @brief Constructs a TransactionService.
     *
     * @param transactionRepo The transaction repository to use.
     */
    TransactionService::TransactionService(
        std::shared_ptr<repo::ITransactionRepo> transactionRepo
    )
        : _transactionRepo(std::move(transactionRepo))
    {
    }

    /**
     * @brief Adds a transaction to the repository.
     *
     * @param transaction The transaction to add.
     * @return CrudResult<TransactionId> The result of the add operation.
     */
    CrudResult<TransactionId> TransactionService::addTransaction(
        const finance::DomainTransaction& transaction
    )
    {
        return _transactionRepo->addTransaction(transaction);
    }

    /**
     * @brief Retrieves all transactions from the repository.
     *
     * @param filter The filter to apply to the transactions, this will be
     * converted to a WhereExpr and applied to the query when fetching
     * transactions from the database, if no filter is provided all
     * transactions will be returned
     *
     * @return std::vector<finance::DomainTransaction> A vector of all
     * transactions.
     */
    std::vector<finance::DomainTransaction> TransactionService::getTransactions(
        const finance::TransactionFilter& filter
    )
    {
        return _transactionRepo->getTransactions(filter);
    }

}   // namespace service
