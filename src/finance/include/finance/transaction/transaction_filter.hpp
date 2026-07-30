#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__

#include "common/container/set.hpp"
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
    struct TransactionFilter
    {
        /// The set of position IDs to filter transactions by
        IdSet<PositionId> positionIds;
        /// The set of transaction IDs to filter transactions by
        IdSet<TransactionId> transactionIds;
        /// The set of account IDs to filter transactions by
        IdSet<AccountId> accountIds;

        [[nodiscard]]
        filter::Predicate<DomainTransaction> getPredicate() const;

        [[nodiscard]]
        std::string toString() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__