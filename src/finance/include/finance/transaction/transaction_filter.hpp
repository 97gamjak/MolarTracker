#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__

#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "utils/container/set.hpp"

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
        IdSet<PositionId> _positionIds;
        /// The set of transaction IDs to filter transactions by
        IdSet<TransactionId> _transactionIds;

       public:
        void setPositionIds(const IdSet<PositionId>& positionIds);
        [[nodiscard]] IdSet<PositionId> getPositionIds() const;

        [[nodiscard]] IdSet<TransactionId> getTransactionIds() const;

        [[nodiscard]]
        filter::Predicate<DomainTransaction> getPredicate() const;

        [[nodiscard]]
        std::string toString() const;
    };

    filter::Predicate<DomainTransaction> HasPositionId(
        const IdSet<PositionId>& positionIds
    );

    filter::Predicate<DomainTransaction> HasTransactionId(
        const IdSet<TransactionId>& transactionIds
    );

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__