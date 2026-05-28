#ifndef __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <vector>

#include "config/id_types.hpp"
#include "finance/transaction.hpp"
#include "finance/transaction_filter.hpp"

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace store
{
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
    class ITransactionStore
    {
       public:
        virtual ~ITransactionStore() = default;

        /**
         * @brief Commit all changes to the database
         *
         * @param accountIdRemap Mapping of account IDs
         * @param instrumentIdRemap Mapping of instrument IDs
         */
        virtual void commit(
            const unorderedIdMap<AccountId, AccountId>&       accountIdRemap,
            const unorderedIdMap<InstrumentId, InstrumentId>& instrumentIdRemap
        ) = 0;

        /**
         * @brief Add a transaction to the store
         *
         * @param transaction The transaction to add
         * @return TransactionStoreResult The result of the operation
         */
        [[nodiscard]]
        virtual TransactionStoreResult addTransaction(
            finance::Transaction transaction
        ) = 0;

        /**
         * @brief Get all transactions in the store
         *
         * @param filter The filter to apply
         *
         * @return std::vector<finance::Transaction> The list of transactions
         */
        [[nodiscard]]
        virtual std::vector<finance::Transaction> getTransactions(
            const finance::TransactionFilter& filter
        ) const = 0;

        /**
         * @brief Get all transactions in the store
         *
         * @return std::vector<finance::Transaction> The list of transactions
         */
        [[nodiscard]]
        virtual std::vector<finance::Transaction> getTransactions() const = 0;

        /**
         * @brief Get all instrument IDs associated with a position ID
         *
         * @param positionId The position ID to filter by
         * @return idSet<InstrumentId> The set of instrument IDs
         */
        [[nodiscard]]
        virtual idSet<InstrumentId> getInstrumentIdsByPositionId(
            PositionId positionId
        ) const = 0;

        /**
         * @brief Find all transactions associated with a position ID
         *
         * @param positionId The position ID to filter by
         * @return std::vector<finance::Transaction> The list of transactions
         */
        [[nodiscard]]
        virtual std::vector<finance::Transaction> findTransactionsByPositionId(
            PositionId positionId
        ) const = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__