#include "app/store/transaction_store.hpp"

#include "app/services_api/i_transaction_service.hpp"
#include "drafts/transaction_mapper.hpp"
#include "finance/transaction.hpp"
#include "logging/log_macros.hpp"

namespace app
{
    /**
     * @brief Construct a new Transaction Store object
     *
     * @param transactionService
     */
    TransactionStore::TransactionStore(
        const std::shared_ptr<ITransactionService>& transactionService
    )
        : _transactionService(transactionService)
    {
    }

    /**
     * @brief Save all temporary changes to the database
     *
     * @param accountIdMap A map of old account IDs to new account IDs, this is
     * used to update the account IDs in the transaction entries before
     * committing them to the database, ensuring that the transactions are
     * associated with the correct accounts after any changes to account IDs
     * have been made.
     */
    void TransactionStore::commit(
        const std::unordered_map<AccountId, AccountId, AccountId::Hash>&
            accountIdMap
    )
    {
        LOG_ENTRY;

        _updateAccountIds(accountIdMap);

        if (!isDirty())
        {
            LOG_DEBUG("Transaction store is not dirty, nothing to commit");
            return;
        }

        for (auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    const auto id =
                        _transactionService->addTransaction(entry.value);

                    LOG_DEBUG(
                        std::format("Added transaction with ID: {}", id.value())
                    );

                    entry.value.setId(id);
                    break;
                }
                case StoreState::Modified:
                case StoreState::Deleted:
                    throw std::runtime_error("Not yet implemented");
                case StoreState::Clean:
                    break;
            }
        }

        _cleanEntries();
    }

    /**
     * @brief Add a transaction to the store, this adds a new transaction to the
     * store in a temporary state, which can then be committed to the database
     * using the commit method. The transaction must have a total sum of zero,
     * meaning that the sum of all entries in the transaction must equal zero,
     * ensuring that the transaction is balanced and does not create or destroy
     * money.
     *
     * @param transaction The transaction to add to the store, this should be a
     * complete transaction with all necessary entries and details filled out,
     * but it will not be saved to the database until the commit method is
     * called.
     * @return TransactionStoreResult The result of the add operation, this will
     * indicate whether the transaction was added successfully or if there was
     * an error (e.g., if the transaction sum is not zero).
     */
    TransactionStoreResult TransactionStore::addTransaction(
        finance::Transaction transaction
    )
    {
        const auto& id = _generateNewId();

        transaction.setId(id);

        const auto cash = transaction.calculateTotalSum();

        if (!cash.isZero())
        {
            LOG_ERROR(
                std::format(
                    "Transaction sum is not zero, cannot add transaction "
                    "draft"
                )
            );
            return TransactionStoreResult::TransactionSumNotZero;
        }

        _addEntry(transaction, StoreState::New);

        return TransactionStoreResult::Ok;
    }

    /**
     * @brief Updates the account IDs in the transaction entries based on the
     * provided account ID map, this is used to ensure that any changes to
     * account IDs are reflected in the transaction entries before they are
     * committed to the database, maintaining the integrity of the associations
     * between transactions and accounts.
     *
     * @param accountIdMap A map of old account IDs to new account IDs, this is
     * used to update the account IDs in the transaction entries, ensuring that
     * the transactions are associated with the correct accounts after any
     * changes to account IDs have been made.
     */
    void TransactionStore::_updateAccountIds(
        const std::unordered_map<AccountId, AccountId, AccountId::Hash>&
            accountIdMap
    )
    {
        for (auto& entry : _getEntries())
        {
            for (auto& transactionEntry : entry.value.getEntries())
            {
                for (const auto& [id, newId] : accountIdMap)
                {
                    if (transactionEntry.getAccountId() == id)
                    {
                        if (entry.state == StoreState::New)
                        {
                            transactionEntry.setAccountId(newId);
                        }
                        else
                        {
                            throw std::runtime_error(
                                "Account ID found in already committed "
                                "transaction "
                                "entry! This is not allowed."
                            );
                        }
                    }
                }
            }
        }
    }

    /**
     * @brief Get all transactions from the store, this retrieves all
     * transactions that are currently in the store, including both new
     * transactions that have not yet been committed to the database and
     * existing transactions that have been loaded from the database. The
     * returned transactions will reflect any changes made to them in the store,
     * but they will not be saved to the database until the commit method is
     * called.
     *
     * @return std::vector<finance::Transaction> A vector of transactions
     * currently in the store, this includes both new and existing transactions,
     * and reflects any changes made to them in the store.
     */
    std::vector<finance::Transaction> TransactionStore::getTransactions() const
    {
        const auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto transactions = _getEntries(options);

        auto dbTransactions = _transactionService->getTransactions();

        // Merge transactions from the database with transactions in the store
        // But check if id is already in the store, if it is, use the one in the
        // store

        std::unordered_map<
            TransactionId,
            finance::Transaction,
            TransactionId::Hash>
            transactionMap;

        std::vector<finance::Transaction> results;

        for (const auto& transaction : transactions)
        {
            // Only include transactions that are new, for all others the id is
            // already in the database and we will get it from there
            results.push_back(transaction.value);

            if (transaction.state != StoreState::New)
            {
                transactionMap.emplace(
                    transaction.value.getId(),
                    transaction.value
                );
            }
        }

        for (const auto& transaction : dbTransactions)
        {
            if (!transactionMap.contains(transaction.getId()))
                results.push_back(transaction);
        }

        return results;
    }

}   // namespace app
