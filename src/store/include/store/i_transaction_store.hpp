#ifndef __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__

#include <cstdint>
#include <mstd/enum.hpp>
#include <vector>

#include "config/id_types.hpp"
#include "finance/transaction/domain_transaction.hpp"   // needed for vector

namespace finance
{
    class Account;             // Forward declaration
    class TransactionFilter;   // Forward declaration
    class Transactions;        // Forward declaration
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
         * @param positionIdRemap Mapping of position IDs
         */
        virtual void commit(
            const unorderedIdMap<AccountId, AccountId>&       accountIdRemap,
            const unorderedIdMap<InstrumentId, InstrumentId>& instrumentIdRemap,
            const unorderedIdMap<PositionId, PositionId>&     positionIdRemap
        ) = 0;

        /**
         * @brief Add a transaction to the store
         *
         * @param transaction The transaction to add
         * @return TransactionStoreResult The result of the operation
         */
        [[nodiscard]]
        virtual TransactionStoreResult addTransaction(
            finance::DomainTransaction transaction
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
         * @return std::vector<finance::DomainTransaction> The list of
         * transactions
         */
        [[nodiscard]]
        virtual std::
            vector<finance::DomainTransaction> findTransactionsByPositionId(
                PositionId positionId
            ) const = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_TRANSACTION_STORE_HPP__