#ifndef __STORE__SRC__STORE__TRANSACTION_STORE_HPP__
#define __STORE__SRC__STORE__TRANSACTION_STORE_HPP__

#include <memory>
#include <mstd/enum.hpp>

#include "config/id_types.hpp"
#include "error/finance_error.hpp"
#include "finance/transaction/domain_transaction.hpp"
#include "finance/transaction/position_transaction.hpp"
#include "finance/transaction/transaction_filter.hpp"
#include "store/base/base_store.hpp"
#include "store/i_transaction_store.hpp"
#include "utils/container/id_id_map.hpp"

namespace finance
{
    class Account;        // Forward declaration
    class AccountsView;   // Forward declaration
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

        /// Connections for various events
        Connections _connections;

       public:
        explicit TransactionStore(
            const std::shared_ptr<service::ITransactionService>&
                transactionService
        );
        ~TransactionStore() override;

        void commit(
            const IdIdMap<AccountId>&    accountIdRemap,
            const IdIdMap<InstrumentId>& instrumentIdRemap,
            const IdIdMap<PositionId>&   positionIdRemap
        );

        [[nodiscard]]
        FinanceResult<void> addCashTransaction(
            const finance::CashTransaction& transaction,
            const finance::AccountsView&    accounts
        ) override;
        [[nodiscard]]
        FinanceResult<void> addStockTransaction(
            const finance::StockTransaction& transaction,
            const finance::AccountsView&     accounts
        ) override;

        [[nodiscard]]
        FinanceResult<void> addOptionTransaction(
            const finance::OptionTransaction& transaction,
            const finance::AccountsView&      accounts
        ) override;

        [[nodiscard]]
        finance::Transactions getTransactions(
            const finance::TransactionFilter& filter,
            const finance::AccountsView&      accounts
        ) const override;
        [[nodiscard]]
        finance::Transactions getTransactions(
            const finance::AccountsView& accounts
        ) const override;

        [[nodiscard]]
        IdMap<PositionId, finance::StockPositionTransaction> getStockPositions(
            const finance::TransactionFilter& filter,
            const finance::AccountsView&      accounts
        ) const override;

        [[nodiscard]]
        Connection subscribeToTransactionAdded(
            OnTransactionAdded::func func,
            void*                    user
        ) override;

       private:
        void _onAccountIdRemap(const IdIdMap<AccountId>& remap);
        void _onInstrumentIdRemap(const IdIdMap<InstrumentId>& remap);
        void _onPositionIdRemap(const IdIdMap<PositionId>& remap);
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__TRANSACTION_STORE_HPP__