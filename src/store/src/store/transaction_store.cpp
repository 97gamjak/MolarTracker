#include "store/transaction_store.hpp"

#include <format>
#include <unordered_map>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "drafts/transaction_mapper.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"
#include "logging/log_macros.hpp"
#include "service/i_transaction_service.hpp"
#include "store/account/account_session.hpp"
#include "store/position_store.hpp"
#include "store/stock_store.hpp"

REGISTER_LOG_CATEGORY("Store.TransactionStore");

namespace store
{

    /**
     * @brief Internal session struct for TransactionStore, this struct holds a
     * reference to the AccountSession and is used to manage the session state
     * of transactions in the store.
     *
     */
    struct TransactionStore::Session
    {
        /// A reference to the AccountSession
        const AccountSession& accountSession;

        /**
         * @brief Construct a new Session object
         *
         * @param accountSession_
         */
        explicit Session(const AccountSession& accountSession_)
            : accountSession(accountSession_)
        {
        }

        ~Session() = default;

        // delete copy and moving
        Session(const Session&)            = delete;
        Session(Session&&)                 = delete;
        Session& operator=(const Session&) = delete;
        Session& operator=(Session&&)      = delete;
    };

    /**
     * @brief Construct a new Transaction Store object
     *
     * @param transactionService
     * @param stockStore
     * @param positionStore
     * @param accountSession
     */
    TransactionStore::TransactionStore(
        const std::shared_ptr<service::ITransactionService>& transactionService,
        StockStore&                                          stockStore,
        PositionStore&                                       positionStore,
        const AccountSession&                                accountSession
    )
        : _transactionService(transactionService),
          _session(std::make_unique<Session>(accountSession))
    {
        _connections.add(stockStore.subscribeToInstrumentIdRemap(
            [this](const instrumentMap<InstrumentId>& remap)
            { _onInstrumentIdRemap(remap); },
            this
        ));

        _connections.add(positionStore.subscribeToIdRemap(
            [this](const PositionStore::IdMap& remap)
            { _onPositionIdRemap(remap); },
            this
        ));
    }

    TransactionStore::~TransactionStore() = default;

    /**
     * @brief Save all temporary changes to the database
     *
     * @param accountIdRemap The account ID remapping to apply during the commit
     */
    void TransactionStore::commit(
        const unorderedIdMap<AccountId, AccountId>& accountIdRemap
    )
    {
        LOG_ENTRY;

        _onAccountIdRemap(accountIdRemap);

        for (const auto& entry : _getEntries())
        {
            switch (entry.state)
            {
                case StoreState::New:
                {
                    LOG_DEBUG(
                        std::format(
                            "Adding new transaction to database: {}",
                            entry.value.toString()
                        )
                    );

                    const auto id =
                        _transactionService->addTransaction(entry.value);

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
     * @param filter An optional filter to apply when retrieving transactions,
     * this allows the caller to specify criteria for which transactions to
     * include in the results, such as filtering by date range, transaction
     * type, or any other relevant attributes of the transactions. If no filter
     * is provided, all transactions in the store will be returned.
     *
     * @return std::vector<finance::Transaction> A vector of transactions
     * currently in the store, this includes both new and existing transactions,
     * and reflects any changes made to them in the store.
     */
    std::vector<finance::Transaction> TransactionStore::getTransactions(
        const finance::TransactionFilter& filter
    ) const
    {
        const auto accountIds = _session->accountSession.getIds();

        if (accountIds.empty())
            return {};

        const auto options = Options{
            .filter   = filter.getPredicate(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        auto transactions = _getEntries(options);

        auto dbTransactions =
            _transactionService->getTransactions(accountIds, filter);

        // Merge transactions from the database with transactions in the store
        // But check if id is already in the store, if it is, use the one in the
        // store
        idSet<TransactionId> transactionIds;

        std::vector<finance::Transaction> results;

        for (const auto& transaction : transactions)
        {
            // Only include transactions that are new, for all others the id is
            // already in the database and we will get it from there
            if (transaction.state != StoreState::New)
                transactionIds.insert(transaction.value.getId());
            else
                results.push_back(transaction.value);
        }

        for (const auto& transaction : dbTransactions)
            if (!transactionIds.contains(transaction.getId()))
                results.push_back(transaction);

        return results;
    }

    /**
     * @brief Find transactions by position ID, this retrieves all transactions
     * that are associated with a specific position ID, allowing the caller to
     * easily find and work with transactions that are related to a particular
     * position. This is useful for analyzing the history of a position,
     * generating reports, or performing any other operations that require
     * access to the transactions associated with a specific position.
     *
     * @param positionId The ID of the position for which to find transactions,
     * this specifies the position that the caller is interested in, and the
     * returned transactions will be those that are associated with this
     * position.
     *
     * @return std::vector<finance::Transaction> A vector of transactions that
     * are associated with the specified position ID, this includes both new and
     * existing transactions that are related to the given position.
     */
    std::vector<finance::Transaction> TransactionStore::
        findTransactionsByPositionId(PositionId positionId) const
    {
        auto filter = finance::TransactionFilter();
        filter.setPositionId(positionId);
        const auto transactions = getTransactions(filter);

        return transactions;
    }

    /**
     * @brief Get a set of instrument IDs associated with a specific position ID
     *
     * @param positionId The ID of the position for which to find instrument IDs
     * @return idSet<InstrumentId> A set of instrument IDs associated with the
     * specified position ID
     */
    idSet<InstrumentId> TransactionStore::getInstrumentIdsByPositionId(
        PositionId positionId
    ) const
    {
        idSet<InstrumentId> instrumentIdSet;

        const auto transactions = findTransactionsByPositionId(positionId);

        for (const auto& transaction : transactions)
        {
            const auto instrumentIds = transaction.getInstrumentIds();

            if (!instrumentIds.empty())
            {
                // This will default construct an
                // empty set if the position ID is
                // not already in the map
                for (const auto& instrumentId : instrumentIds)
                    instrumentIdSet.insert(instrumentId);
            }
        }

        return instrumentIdSet;
    }

    /**
     * @brief Handle account ID remapping for transaction entries
     *
     * @param remap The mapping of old account IDs to new account IDs
     */
    void TransactionStore::_onAccountIdRemap(const accountMap<AccountId>& remap)
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
        LOG_ENTRY;

        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the remapped
                // ID
                const auto hasId = finance::hasId(
                    entry.value.getData(),
                    remap,
                    &finance::TradeLeg::getInstrumentId
                );

                if (hasId)
                {
                    throw std::runtime_error(
                        "Instrument ID found in already committed "
                        "transaction "
                        "entry!"
                    );
                }

                continue;
            }

            switch (entry.value.getType())
            {
                case TransactionDataType::Trade:
                {
                    auto  transaction = entry.value;
                    auto& data =
                        std::get<finance::TradeData>(transaction.getData());

                    bool modified = false;

                    for (auto& leg : data.getLegs())
                    {
                        if (const auto it = remap.find(leg.getInstrumentId());
                            it != remap.end())
                        {
                            LOG_DEBUG(
                                std::format(
                                    "Remapping instrument ID in transaction "
                                    "leg "
                                    "{}: "
                                    "{} -> "
                                    "{}",
                                    leg.toString(),
                                    leg.getInstrumentId().toString(),
                                    it->second.toString()
                                )
                            );
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

    /**
     * @brief Handle position ID remapping for transaction entries
     *
     * @param remap The mapping of old position IDs to new position IDs
     */
    void TransactionStore::_onPositionIdRemap(
        const positionMap<PositionId>& remap
    )
    {
        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the remapped
                // ID
                const auto hasId = finance::hasId(
                    entry.value.getData(),
                    remap,
                    &finance::TradeLeg::getPositionId
                );

                if (hasId)
                {
                    throw std::runtime_error(
                        "Position ID found in already committed "
                        "transaction "
                        "entry!"
                    );
                }
                continue;
            }

            switch (entry.value.getType())
            {
                case TransactionDataType::Trade:
                {
                    auto  transaction = entry.value;
                    auto& data =
                        std::get<finance::TradeData>(transaction.getData());

                    bool modified = false;

                    for (auto& leg : data.getLegs())
                    {
                        if (const auto it = remap.find(leg.getPositionId());
                            it != remap.end())
                        {
                            LOG_DEBUG(
                                std::format(
                                    "Remapping position ID in transaction leg "
                                    "{}: "
                                    "{} -> "
                                    "{}",
                                    leg.toString(),
                                    leg.getPositionId().toString(),
                                    it->second.toString()
                                )
                            );
                            leg.setPositionId(it->second);
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

}   // namespace store
