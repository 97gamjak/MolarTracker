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
