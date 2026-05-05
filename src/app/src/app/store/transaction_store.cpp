#include "app/store/transaction_store.hpp"

#include <unordered_map>

#include "app/services_api/i_transaction_service.hpp"
#include "app/store/account_store.hpp"
#include "app/store/stock_store.hpp"
#include "config/id_types.hpp"
#include "drafts/transaction_mapper.hpp"
#include "finance/transaction.hpp"
#include "logging/log_macros.hpp"

REGISTER_LOG_CATEGORY("App.Store.TransactionStore");

namespace app
{
    /**
     * @brief Construct a new Transaction Store object
     *
     * @param transactionService
     * @param accountStore
     * @param stockStore
     */
    TransactionStore::TransactionStore(
        const std::shared_ptr<ITransactionService>& transactionService,
        AccountStore&                               accountStore,
        StockStore&                                 stockStore
    )
        : _transactionService(transactionService)
    {
        _connections.add(accountStore.subscribeToIdRemap(
            [this](const AccountStore::IdMap& remap)
            { _onAccountIdRemap(remap); },
            this
        ));

        _connections.add(stockStore.subscribeToInstrumentIdRemap(
            [this](const instrumentMap<InstrumentId>& remap)
            { _onInstrumentIdRemap(remap); },
            this
        ));
    }

    /**
     * @brief Save all temporary changes to the database
     *
     */
    void TransactionStore::commit()
    {
        LOG_ENTRY;

        for (const auto& entry : _getEntries())
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

                    auto persisted = entry.value;
                    persisted.setId(id);
                    _commitEntry(
                        entry.value.getId(),
                        Entry{.value = persisted, .state = entry.state}
                    );
                    break;
                }
                case StoreState::Modified:
                case StoreState::Deleted:
                    throw std::runtime_error("Not yet implemented");
                case StoreState::Clean:
                    break;
            }
        }

        _notifyOnCommit();
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
        LOG_ENTRY;

        const auto cash = transaction.calculateTotalSum();

        if (!cash.isZero())
        {
            LOG_ERROR(
                std::format(
                    "Transaction sum is not zero (={}), cannot add transaction "
                    "draft",
                    cash.toString()
                )
            );
            return TransactionStoreResult::TransactionSumNotZero;
        }

        _addEntry(std::move(transaction));

        return TransactionStoreResult::Ok;
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

    /**
     * @brief Handle account ID remapping for transaction entries
     *
     * @param remap The mapping of old account IDs to new account IDs
     */
    void TransactionStore::_onAccountIdRemap(const AccountStore::IdMap& remap)
    {
        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the remapped
                // ID
                const auto references = std::ranges::any_of(
                    entry.value.getEntries(),
                    [&remap](const auto& entry_)
                    { return remap.contains(entry_.getAccountId()); }
                );

                if (references)
                {
                    throw std::runtime_error(
                        "Account ID found in already committed transaction "
                        "entry!"
                    );
                }

                continue;
            }

            bool modified    = false;
            auto transaction = entry.value;

            for (auto& transactionEntry : transaction.getEntries())
            {
                if (const auto it = remap.find(transactionEntry.getAccountId());
                    it != remap.end())
                {
                    transactionEntry.setAccountId(it->second);
                    modified = true;
                }
            }

            if (modified)
                _updateEntry(transaction, StoreState::New);
        }
    }

    /**
     * @brief Handle instrument ID remapping for transaction entries
     *
     * @param remap The mapping of old instrument IDs to new instrument IDs
     */
    void TransactionStore::_onInstrumentIdRemap(
        const instrumentMap<InstrumentId>& remap
    )
    {
        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the remapped
                // ID
                switch (entry.value.getType())
                {
                    case TransactionDataType::Trade:
                    {
                        const auto data =
                            std::get<finance::TradeData>(entry.value.getData());

                        const auto references = std::ranges::any_of(
                            data.getLegs(),
                            [&remap](const auto& leg)
                            { return remap.contains(leg.getInstrumentId()); }
                        );

                        if (references)
                        {
                            throw std::runtime_error(
                                "Instrument ID found in already committed "
                                "transaction "
                                "entry!"
                            );
                        }
                        break;
                    }
                    case TransactionDataType::Cash:
                        break;
                }

                continue;
            }

            switch (entry.value.getType())
            {
                case TransactionDataType::Trade:
                {
                    auto transaction = entry.value;
                    auto data =
                        std::get<finance::TradeData>(transaction.getData());

                    bool modified = false;

                    for (auto& leg : data.getLegs())
                    {
                        if (const auto it = remap.find(leg.getInstrumentId());
                            it != remap.end())
                        {
                            leg.setInstrumentId(it->second);
                            modified = true;
                        }
                    }

                    if (modified)
                        _updateEntry(transaction, StoreState::New);
                    break;
                }
                case TransactionDataType::Cash:
                    break;
            }
        }
    }

}   // namespace app
