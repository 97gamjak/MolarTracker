#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__

#include "config/id_types.hpp"
#include "filter/predicate.hpp"

namespace finance
{
    class DomainTransaction;

    /**
     * @brief Filter for transactions, used to filter transactions when querying
     * the transaction store
     *
     */
    class TransactionFilter
    {
       private:
        /// The set of position IDs to filter transactions by
        idSet<PositionId> _positionIds;
        /// The set of transaction IDs to filter transactions by
        idSet<TransactionId> _transactionIds;

       public:
        TransactionFilter() = default;

        void setPositionIds(const idSet<PositionId>& positionIds);
        [[nodiscard]] idSet<PositionId> getPositionIds() const;

        [[nodiscard]] idSet<TransactionId> getTransactionIds() const;

        [[nodiscard]]
        filter::Predicate<DomainTransaction> getPredicate() const;

        [[nodiscard]]
        std::string toString() const;
    };

    filter::Predicate<DomainTransaction> HasPositionId(
        const idSet<PositionId>& positionIds
    );

    filter::Predicate<DomainTransaction> HasTransactionId(
        const idSet<TransactionId>& transactionIds
    );

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__