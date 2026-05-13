#ifndef __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__

#include <memory>
#include <vector>

#include "base/base_store.hpp"
#include "config/id_types.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"

namespace app
{
    class AccountStore;          // Forward declaration
    class PositionStore;         // Forward declaration
    class StockStore;            // Forward declaration
    class ITransactionService;   // Forward declaration

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
        std::shared_ptr<ITransactionService> _transactionService;

        /// Connections for various events
        Connections _connections;

       public:
        explicit TransactionStore(
            const std::shared_ptr<ITransactionService>& transactionService,
            AccountStore&                               accountStore,
            StockStore&                                 stockStore,
            PositionStore&                              positionStore
        );

        void commit();

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

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__TRANSACTION_STORE_HPP__