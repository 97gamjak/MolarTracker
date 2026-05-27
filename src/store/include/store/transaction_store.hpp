#ifndef __STORE__INCLUDE__STORE__TRANSACTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__TRANSACTION_STORE_HPP__

#include <cstdint>
#include <memory>
#include <mstd/enum.hpp>
#include <vector>

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"

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

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TRANSACTION_STORE_RESULT(X) \
    X(Ok)                           \
    X(Error)                        \
    X(TransactionSumNotZero)

    MSTD_ENUM(TransactionStoreResult, std::uint8_t, TRANSACTION_STORE_RESULT);

    /**
     * @brief Store for managing transactions
     *
     */
    class TransactionStore
        : public BaseStore<finance::Transaction, TransactionId>
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
        );

        [[nodiscard]]
        TransactionStoreResult addTransaction(finance::Transaction transaction);

        [[nodiscard]]
        std::vector<finance::Transaction> getTransactions(
            const finance::TransactionFilter& filter =
                finance::TransactionFilter()
        ) const;
        [[nodiscard]]
        idSet<InstrumentId> getInstrumentIdsByPositionId(
            PositionId positionId
        ) const;
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

#endif   // __STORE__INCLUDE__STORE__TRANSACTION_STORE_HPP__