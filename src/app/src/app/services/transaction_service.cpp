#include "transaction_service.hpp"

#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"
#include "repo/i_transaction_repo.hpp"

namespace app
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
     * @param accountIds The IDs of the accounts to retrieve transactions
     * for.
     * @param filter The filter to apply to the transactions, this will be
     * converted to a WhereExpr and applied to the query when fetching
     * transactions from the database, if no filter is provided all
     * transactions will be returned
     *
     * @return std::vector<finance::Transaction> A vector of all transactions.
     */
    std::vector<finance::Transaction> TransactionService::getTransactions(
        const idSet<AccountId>&           accountIds,
        const finance::TransactionFilter& filter
    )
    {
        return _transactionRepo->getTransactions(accountIds, filter);
    }

}   // namespace app
