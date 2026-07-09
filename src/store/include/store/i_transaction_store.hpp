#ifndef __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__

#include <functional>
#include <mstd/enum.hpp>

#include "config/id_types.hpp"
#include "error/finance_error.hpp"
#include "finance/transaction/position_transaction.hpp"   // needed for vector
#include "utils/container/id_map.hpp"

namespace finance
{
    class Account;             // Forward declaration
    class AccountsView;        // Forward declaration
    class TransactionFilter;   // Forward declaration
    class Transactions;        // Forward declaration
    class OptionTransaction;   // Forward declaration
}   // namespace finance

class Connection;   // Forward declaration

namespace store
{
    class ITransactionStoreReader
    {
       protected:
        /**
         * @brief Structure representing a callback for when a transaction is
         * added.
         *
         */
        struct OnTransactionAdded
        {
            /// The callback function type for when a transaction is added
            using func =
                std::function<void(finance::DomainTransaction transaction)>;
        };

       public:
        virtual ~ITransactionStoreReader() = default;

        [[nodiscard]]
        virtual std::optional<finance::DomainTransaction> getTransaction(
            TransactionId transactionId
        ) const = 0;

        /**
         * @brief Get all transactions in the store
         *
         * @return std::vector<finance::DomainTransaction> The list of
         * transactions
         */
        [[nodiscard]]
        virtual std::vector<finance::DomainTransaction> getTransactions(
            const finance::TransactionFilter& filter,
            const IdSet<AccountId>&           accountIds
        ) const = 0;

        /**
         * @brief Get the Stock Positions
         *
         * @param filter
         * @param accounts
         * @return IdMap<PositionId, finance::StockPositionTransaction>
         */
        [[nodiscard]]
        virtual IdMap<PositionId, finance::StockPositionTransaction> getStockPositions(
            const finance::TransactionFilter& filter,
            const finance::AccountsView&      accounts
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

    /**
     * @brief Store for managing transactions
     *
     */
    class ITransactionStore
    {
       public:
        virtual ~ITransactionStore() = default;

        /**
         * @brief Add a cash transaction to the store
         *
         * @param transaction The cash transaction to add
         * @param accounts The accounts view to filter transactions
         * @return FinanceResult<void> The result of the operation
         */
        [[nodiscard]]
        virtual FinanceResult<void> addCashTransaction(
            const finance::CashTransaction& transaction,
            const finance::AccountsView&    accounts
        ) = 0;

        /**
         * @brief Add a stock transaction to the store
         *
         * @param transaction The stock transaction to add
         * @param accounts The accounts view to filter transactions
         * @return FinanceResult<void> The result of the operation
         */
        [[nodiscard]]
        virtual FinanceResult<void> addStockTransaction(
            const finance::StockTransaction& transaction,
            const finance::AccountsView&     accounts
        ) = 0;

        /**
         * @brief Add an option transaction to the store
         *
         * @param transaction The option transaction to add
         * @param accounts The accounts view to filter transactions
         * @return FinanceResult<void> The result of the operation
         */
        [[nodiscard]]
        virtual FinanceResult<void> addOptionTransaction(
            const finance::OptionTransaction& transaction,
            const finance::AccountsView&      accounts
        ) = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__