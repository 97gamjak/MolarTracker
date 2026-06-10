#ifndef __STORE__SRC__STORE__TRANSACTION_STORE_HPP__
#define __STORE__SRC__STORE__TRANSACTION_STORE_HPP__

#include <memory>
#include <mstd/enum.hpp>

#include "config/id_types.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/position_transaction.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "store/base/base_store.hpp"
#include "store/i_transaction_store.hpp"

namespace finance
{
    class Account;    // Forward declaration
    class Accounts;   // Forward declaration
}   // namespace finance

namespace service
{
    class ITransactionService;   // Forward declaration
}   // namespace service

namespace store
{
    class PositionStore;   // Forward declaration

    /**
     * @brief Store for managing transactions
     *
     */
    class TransactionStore
        : public BaseStore<finance::DomainTransaction, TransactionId>,
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
            const finance::Accounts& accountSession
        );
        ~TransactionStore() override;

        void commit(
            const unorderedIdMap<AccountId, AccountId>&       accountIdRemap,
            const unorderedIdMap<InstrumentId, InstrumentId>& instrumentIdRemap,
            const unorderedIdMap<PositionId, PositionId>&     positionIdRemap
        ) override;

        [[nodiscard]]
        TransactionStoreResult addCashTransaction(
            finance::CashTransaction transaction
        ) override;
        [[nodiscard]]
        TransactionStoreResult addStockTransaction(
            finance::StockTransaction transaction
        ) override;

        [[nodiscard]]
        finance::Transactions getTransactions(
            const finance::TransactionFilter& filter
        ) const override;
        [[nodiscard]]
        finance::Transactions getTransactions() const override;

        [[nodiscard]]
        unorderedIdMap<PositionId, finance::StockPositionTransaction> getStockPositions(
            const finance::TransactionFilter& filter
        ) const override;

        [[nodiscard]]
        Connection subscribeToTransactionAdded(
            OnTransactionAdded::func func,
            void*                    user
        ) override;

       private:
        void _onAccountIdRemap(
            const unorderedIdMap<AccountId, AccountId>& remap
        );
        void _onInstrumentIdRemap(
            const unorderedIdMap<InstrumentId, InstrumentId>& remap
        );
        void _onPositionIdRemap(
            const unorderedIdMap<PositionId, PositionId>& remap
        );
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__TRANSACTION_STORE_HPP__