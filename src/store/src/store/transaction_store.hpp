#ifndef __STORE__SRC__STORE__TRANSACTION_STORE_HPP__
#define __STORE__SRC__STORE__TRANSACTION_STORE_HPP__

#include <memory>
#include <mstd/enum.hpp>
#include <vector>

#include "config/id_types.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"
#include "store/base/base_store.hpp"
#include "store/i_transaction_store.hpp"

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace service
{
    class ITransactionService;   // Forward declaration
}   // namespace service

namespace store
{
    class AccountSession;   // Forward declaration
    class PositionStore;    // Forward declaration

    /**
     * @brief Store for managing transactions
     *
     */
    class TransactionStore
        : public BaseStore<finance::Transaction, TransactionId>,
          public ITransactionStore
    {
       private:
        /// The Transaction service
        std::shared_ptr<service::ITransactionService> _transactionService;

        struct Session;
        /// The session object for managing the session state of transactions in
        /// the store
        std::unique_ptr<Session> _session;

        /// Connections for various events
        Connections _connections;

       public:
        explicit TransactionStore(
            const std::shared_ptr<service::ITransactionService>&
                                  transactionService,
            PositionStore&        positionStore,
            const AccountSession& accountSession
        );
        ~TransactionStore() override;

        void commit(
            const unorderedIdMap<AccountId, AccountId>&       accountIdRemap,
            const unorderedIdMap<InstrumentId, InstrumentId>& instrumentIdRemap
        ) override;

        [[nodiscard]]
        TransactionStoreResult addTransaction(
            finance::Transaction transaction
        ) override;

        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions(
            const finance::TransactionFilter& filter
        ) const override;
        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions() const override;

        [[nodiscard]]
        idSet<InstrumentId> getInstrumentIdsByPositionId(
            PositionId positionId
        ) const override;

        [[nodiscard]]
        std::vector<finance::Transaction> findTransactionsByPositionId(
            PositionId positionId
        ) const;

       private:
        void _onAccountIdRemap(const accountMap<AccountId>& remap);
        void _onInstrumentIdRemap(const instrumentMap<InstrumentId>& remap);
        void _onPositionIdRemap(const positionMap<PositionId>& remap);
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__TRANSACTION_STORE_HPP__