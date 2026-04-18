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
