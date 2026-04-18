#include "app/store/transaction_store.hpp"

#include "drafts/transaction_draft.hpp"
#include "drafts/transaction_mapper.hpp"
#include "finance/transaction.hpp"
#include "logging/log_macros.hpp"

using drafts::TransactionMapper;

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
    }

    /**
     * @brief Add a transaction to the store, this will add the transaction as
     * a new entry in the store and mark it as New, indicating that it has not
     * yet been committed to the database, and will be included in the next
     * commit operation.
     *
     * @param draft The transaction draft to add to the store, this contains the
     * details of the transaction to be added, including its entries and other
     * relevant information needed to create a complete transaction in the
     * store.
     * @return TransactionStoreResult The result of the add operation,
     * indicating whether it was successful or if there were any issues (e.g.,
     * if the transaction sum is not zero).
     */
    TransactionStoreResult TransactionStore::addTransaction(
        const drafts::TransactionDraft& draft
    )
    {
        const auto& id = _generateNewId();

        auto cash = finance::Cash(draft.entries.front().cash.getCurrency(), 0);

        for (const auto& entry : draft.entries)
            cash += entry.cash;

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

        _addEntry(TransactionMapper::toTransaction(draft, id), StoreState::New);

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
     * @brief Retrieves all transactions from the store, this will return a
     * vector of transaction drafts representing the transactions currently in
     * the store, including any new transactions that have been added but not
     * yet committed to the database, as well as transactions that are already
     * in the database but may have been modified in the store.
     *
     * @return std::vector<drafts::TransactionDraft> A vector of transaction
     * drafts representing the transactions in the store.
     */
    std::vector<drafts::TransactionDraft> TransactionStore::getTransactions(
    ) const
    {
        const auto options = Options{.deletion = DeletionPolicy::ExcludeDelete};

        auto transactions = _getEntries(options);

        auto dbTransactions = _transactionService->getTransactions();

        // Merge transactions from the database with transactions in the store
        // But check if id is already in the store, if it is, use the one in the
        // store

        std::unordered_map<
            TransactionId,
            drafts::TransactionDraft,
            TransactionId::Hash>
            transactionMap;

        std::vector<drafts::TransactionDraft> results;

        for (const auto& transaction : transactions)
        {
            // Only include transactions that are new, for all others the id is
            // already in the database and we will get it from there
            const auto draft = TransactionMapper::toDraft(transaction.value);

            results.push_back(draft);

            if (transaction.state != StoreState::New)
            {
                transactionMap.emplace(transaction.value.getId(), draft);
            }
        }

        for (const auto& transaction : dbTransactions)
        {
            if (!transactionMap.contains(transaction.getId()))
                results.push_back(TransactionMapper::toDraft(transaction));
        }

        return results;
    }

}   // namespace app
