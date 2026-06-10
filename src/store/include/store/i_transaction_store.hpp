#ifndef __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__

#include <cstdint>
#include <functional>
#include <mstd/enum.hpp>

#include "config/id_types.hpp"
#include "finance/transaction/position_transaction.hpp"   // needed for vector

namespace finance
{
    class Account;             // Forward declaration
    class TransactionFilter;   // Forward declaration
    class Transactions;        // Forward declaration
}   // namespace finance

class Connection;   // Forward declaration

namespace store
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define TRANSACTION_STORE_RESULT(X) \
    X(Ok)                           \
    X(Error)                        \
    X(TransactionSumNotZero)

    MSTD_ENUM(TransactionStoreResult, std::uint8_t, TRANSACTION_STORE_RESULT);

    /**
     * @brief Structure representing a callback for when a transaction is added.
     *
     */
    struct OnTransactionAdded
    {
        /// The callback function type for when a transaction is added
        using func = std::function<void(finance::Transactions transactions)>;
    };

    /**
     * @brief Store for managing transactions
     *
     */
    class ITransactionStore
    {
       public:
        virtual ~ITransactionStore() = default;

        /**
         * @brief Commit all changes to the database
         *
         * @param accountIdRemap Mapping of account IDs
         * @param instrumentIdRemap Mapping of instrument IDs
         * @param positionIdRemap Mapping of position IDs
         */
        virtual void commit(
            const unorderedIdMap<AccountId, AccountId>&       accountIdRemap,
            const unorderedIdMap<InstrumentId, InstrumentId>& instrumentIdRemap,
            const unorderedIdMap<PositionId, PositionId>&     positionIdRemap
        ) = 0;

        /**
         * @brief Add a cash transaction to the store
         *
         * @param transaction The cash transaction to add
         * @return TransactionStoreResult The result of the operation
         */
        [[nodiscard]]
        virtual TransactionStoreResult addCashTransaction(
            finance::CashTransaction transaction
        ) = 0;

        /**
         * @brief Add a stock transaction to the store
         *
         * @param transaction The stock transaction to add
         * @return TransactionStoreResult The result of the operation
         */
        [[nodiscard]]
        virtual TransactionStoreResult addStockTransaction(
            finance::StockTransaction transaction
        ) = 0;

        /**
         * @brief Get all transactions in the store
         *
         * @param filter The filter to apply
         *
         * @return finance::Transactions The list of
         * transactions
         */
        [[nodiscard]]
        virtual finance::Transactions getTransactions(
            const finance::TransactionFilter& filter
        ) const = 0;

        /**
         * @brief Get all transactions in the store
         *
         * @return finance::Transactions The list of
         * transactions
         */
        [[nodiscard]]
        virtual finance::Transactions getTransactions() const = 0;

        /**
         * @brief Get the Stock Positions
         *
         * @param filter
         * @return unorderedIdMap<PositionId, finance::StockPositionTransaction>
         */
        [[nodiscard]]
        virtual unorderedIdMap<PositionId, finance::StockPositionTransaction> getStockPositions(
            const finance::TransactionFilter& filter
        ) const = 0;

        /**
         * @brief Subscribe to transaction added events, this allows subscribers
         * to be notified when a transaction is added, which can be useful for
         * updating the UI or performing other actions in response to a
         * transaction being added.
         *
         * @param func The callback function to call when a transaction is added
         * @param user A pointer to the user data, which will be passed to the
         * callback function
         * @return Connection The connection object, which can be used to
         * unsubscribe from the event
         */
        [[nodiscard]]
        virtual Connection subscribeToTransactionAdded(
            OnTransactionAdded::func func,
            void*                    user
        ) = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__