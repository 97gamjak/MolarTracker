#include "store/transaction_store.hpp"

#include <format>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "finance/account/accounts.hpp"
#include "finance/transaction/cash_transaction.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/transaction_converter.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "finance/transaction/transactions.hpp"
#include "logging/log_macros.hpp"
#include "service/i_transaction_service.hpp"
#include "store/i_stock_store.hpp"

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
        const finance::Accounts& accountSession;

        /**
         * @brief Construct a new Session object
         *
         * @param accountSession_
         */
        explicit Session(const finance::Accounts& accountSession_)
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
     * @param accountSession
     */
    TransactionStore::TransactionStore(
        const std::shared_ptr<service::ITransactionService>& transactionService,
        const finance::Accounts&                             accountSession
    )
        : _transactionService(transactionService),
          _session(std::make_unique<Session>(accountSession))
    {
    }

    TransactionStore::~TransactionStore() = default;

    /**
     * @brief Save all temporary changes to the database
     *
     * @param accountIdRemap The account ID remapping to apply during the commit
     * @param instrumentIdRemap A mapping of old instrument IDs to new
     * instrument IDs, this is used to update any transactions in the store that
     * reference instrument IDs that have been remapped, ensuring that the
     * transactions remain consistent with the current state of the instruments
     * in the store and the database after a commit operation that may have
     * resulted in changes to instrument IDs. This allows the TransactionStore
     * to maintain the integrity of its transactions and their references to
     * instruments, even when instrument IDs are changed during a commit.
     * @param positionIdRemap Mapping of position IDs
     */
    void TransactionStore::commit(
        const IdIdMap<AccountId>&    accountIdRemap,
        const IdIdMap<InstrumentId>& instrumentIdRemap,
        const IdIdMap<PositionId>&   positionIdRemap
    )
    {
        LOG_ENTRY;

        _logCache(LOG_CATEGORY, LogLevel::Trace);

        if (!isDirty())
        {
            LOG_DEBUG("No changes to commit");
            return;
        }

        _onAccountIdRemap(accountIdRemap);
        _onInstrumentIdRemap(instrumentIdRemap);
        _onPositionIdRemap(positionIdRemap);

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

                    const auto oldId = entry.value.getId();
                    const auto id =
                        _transactionService->addTransaction(entry.value);

                    auto persisted = entry.value;
                    persisted.setId(id);
                    _commitEntry(
                        oldId,
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

        _logCache(LOG_CATEGORY, LogLevel::Trace);

        _notifyOnCommit();
    }

    /**
     * @brief Add a cash transaction to the store
     *
     * @param transaction The cash transaction to add
     * @return TransactionStoreResult The result of the operation
     */
    TransactionStoreResult TransactionStore::addCashTransaction(
        finance::CashTransaction transaction
    )
    {
        LOG_ENTRY;

        _addEntry(
            finance::TransactionConverter::toDomain(
                transaction,
                _session->accountSession
            )
        );

        return TransactionStoreResult::Ok;
    }

    /**
     * @brief Add a stock transaction to the store
     *
     * @param transaction The stock transaction to add
     * @return TransactionStoreResult The result of the operation
     */
    TransactionStoreResult TransactionStore::addStockTransaction(
        finance::StockTransaction transaction
    )
    {
        LOG_ENTRY;

        _addEntry(
            finance::TransactionConverter::toDomain(
                transaction,
                _session->accountSession
            )
        );

        return TransactionStoreResult::Ok;
    }

    TransactionStoreResult TransactionStore::addOptionTransaction(
        finance::OptionTransaction transaction
    )
    {
        LOG_ENTRY;

        _addEntry(
            finance::TransactionConverter::toDomain(
                transaction,
                _session->accountSession
            )
        );

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
     * @return FinanceResult<finance::Transactions>
     */
    FinanceResult<finance::Transactions> TransactionStore::getTransactions(
    ) const
    {
        return getTransactions(finance::TransactionFilter());
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
     * @return FinanceResult<finance::Transactions>
     */
    FinanceResult<finance::Transactions> TransactionStore::getTransactions(
        finance::TransactionFilter filter
    ) const
    {
        const auto accountIds = _session->accountSession.getIds();

        if (accountIds.empty())
            return {};

        if (filter.accountIds.empty())
            filter.accountIds = accountIds;

        const auto options = Options{
            .filter   = filter.getPredicate(),
            .deletion = DeletionPolicy::ExcludeDelete
        };

        LOG_DEBUG(
            std::format(
                "Retrieving transactions with filter: {}",
                filter.toString()
            )
        );
        auto transactions = _getEntries(options);

        // Merge transactions from the database with transactions in the store
        // But check if id is already in the store, if it is, use the one in the
        // store
        IdSet<TransactionId> transactionIds;

        std::vector<finance::DomainTransaction> results;
        for (const auto& transaction : transactions)
        {
            transactionIds.insert(transaction.value.getId());
            results.push_back(transaction.value);
        }

        LOG_DEBUG(
            std::format(
                "Transactions retrieved from store: {}, with ids: {}",
                results.size(),
                transactionIds.toString()
            )
        );

        auto dbTransactions = _transactionService->getTransactions(filter);

        LOG_DEBUG(
            std::format(
                "Transactions retrieved from database: {} with ids: {}",
                dbTransactions.size(),
                IdSet<TransactionId>::fromRange(
                    dbTransactions,
                    [](const auto& tx) { return tx.getId(); }
                ).toString()
            )
        );

        for (const auto& transaction : dbTransactions)
            if (!transactionIds.contains(transaction.getId()))
                results.push_back(transaction);

        finance::Transactions txs;
        const auto&           result =
            txs.addTransactions(results, _session->accountSession);

        if (!result)
        {
            const auto& error = result.error().convert(
                FinanceErrorType::InvalidTransaction,
                "Failed to add transactions to Transactions object"
            );
            LOG_ERROR(error.toString());
            return error;
        }

        LOG_DEBUG(
            std::format(
                "Transactions retrieved: stocks({}), cash({}), options({})",
                txs.stocks().size(),
                txs.cash().size(),
                txs.options().size()
            )
        );
        return txs;
    }

    /**
     * @brief Handle account ID remapping for transaction entries
     *
     * @param remap The mapping of old account IDs to new account IDs
     */
    void TransactionStore::_onAccountIdRemap(const IdIdMap<AccountId>& remap)
    {
        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the
                // remapped ID
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

            bool modified = false;

            auto txEntries = entry.value.getEntries();
            for (auto& txEntry : txEntries)
            {
                const auto id = txEntry.getAccountId();
                if (remap.contains(id))
                {
                    txEntry.setAccountId(remap.at(id));
                    modified = true;
                }
            }

            auto legs = entry.value.getLegs();
            for (auto& leg : legs)
            {
                const auto id = leg.getAccountId();
                if (remap.contains(id))
                {
                    leg.setAccountId(remap.at(id));
                    modified = true;
                }
            }

            if (modified)
            {
                auto txCopy = entry.value;
                txCopy.setEntries(txEntries);
                txCopy.setLegs(legs);
                _updateEntry(txCopy, StoreState::New);
            }
        }
    }

    /**
     * @brief Handle instrument ID remapping for transaction entries
     *
     * @param remap The mapping of old instrument IDs to new instrument IDs
     */
    void TransactionStore::_onInstrumentIdRemap(
        const IdIdMap<InstrumentId>& remap
    )
    {
        LOG_ENTRY;

        LOG_TRACE(
            std::format(
                "Remapping instrument IDs in transactions: {}",
                remap.toString()
            )
        );

        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the
                // remapped ID
                const auto hasId = std::ranges::any_of(
                    remap,
                    [&entry](const auto& pair)
                    { return entry.value.hasInstrumentId(pair.first); }
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

            bool               modified = false;
            finance::TradeLegs legs     = entry.value.getLegs();

            for (auto& leg : legs)
            {
                const auto instrumentId = leg.getInstrumentId();
                if (remap.contains(instrumentId))
                {
                    LOG_DEBUG(
                        std::format(
                            "Remapping instrument ID in transaction leg {}: {} "
                            "-> {}",
                            leg.toString(),
                            instrumentId.toString(),
                            remap.at(instrumentId).toString()
                        )
                    );
                    leg.setInstrumentId(remap.at(instrumentId));
                    modified = true;
                }
            }

            if (modified)
            {
                auto txCopy = entry.value;
                txCopy.setLegs(legs);
                _updateEntry(txCopy, StoreState::New);
            }
        }
    }

    /**
     * @brief Handle position ID remapping for transaction entries
     *
     * @param remap The mapping of old position IDs to new position IDs
     */
    void TransactionStore::_onPositionIdRemap(const IdIdMap<PositionId>& remap)
    {
        LOG_TRACE(
            std::format(
                "Remapping position IDs in transactions: {}",
                remap.toString()
            )
        );

        for (const auto& entry : _getEntries())
        {
            if (entry.state != StoreState::New)
            {
                // check if this committed transaction references the
                // remapped ID
                const auto hasId = std::ranges::any_of(
                    remap,
                    [&entry](const auto& pair)
                    { return entry.value.hasPositionId(pair.first); }
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

            bool modified = false;

            auto legs = entry.value.getLegs();
            for (auto& leg : legs)
            {
                const auto positionId = leg.getPositionId();
                if (remap.contains(positionId))
                {
                    const auto newPositionId = remap.at(positionId);

                    LOG_DEBUG(
                        std::format(
                            "Remapping position ID in transaction "
                            "leg "
                            "{}: "
                            "{} -> "
                            "{}",
                            leg.toString(),
                            positionId.toString(),
                            newPositionId.toString()
                        )
                    );
                    leg.setPositionId(newPositionId);
                    modified = true;
                }
            }

            if (modified)
            {
                auto txCopy = entry.value;
                txCopy.setLegs(legs);
                _updateEntry(txCopy, StoreState::New);
            }
        }
    }

    Connection TransactionStore::subscribeToTransactionAdded(
        OnTransactionAdded::func func,
        void*                    user
    )
    {
        return subscribeToEntryAdded(
            [func = std::move(func),
             this](const std::vector<finance::DomainTransaction>& transactions)
            {
                func(
                    finance::Transactions(
                        transactions,
                        _session->accountSession
                    )
                );
            },
            user
        );
    }

    /**
     * @brief Discard all cached transactions. Transactions load lazily, so
     * clearing the cache is sufficient — the next query will fetch from the
     * restored database.
     */
    void TransactionStore::reload()
    {
        LOG_ENTRY;
        _logCache(LOG_CATEGORY, LogLevel::Debug);
        _clearEntries();
    }

}   // namespace store
