#include "app/store/transaction_store.hpp"

#include "config/finance.hpp"
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
     */
    void TransactionStore::commit(
        const std::unordered_map<AccountId, AccountId, AccountId::Hash>&
            accountIdMap
    )
    {
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

        _addEntry(
            finance::Transaction(
                id,
                draft.timestamp,
                TransactionStatus::Completed
            ),
            StoreState::New
        );

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

}   // namespace app
