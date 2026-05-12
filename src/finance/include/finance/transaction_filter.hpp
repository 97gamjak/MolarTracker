#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_FILTER_HPP__

#include <optional>

#include "config/id_types.hpp"
#include "filter/predicate.hpp"

namespace finance
{
    class Transaction;

    /**
     * @brief Filter for transactions, used to filter transactions when querying
     * the transaction store
     *
     */
    class TransactionFilter
    {
       private:
        /// An optional position ID to filter transactions by, if set, only
        /// transactions that are associated with the specified position ID will
        /// be included in the results when this filter is applied. If
        /// std::nullopt, no filtering by position ID will be applied.
        std::optional<PositionId> _positionId;

       public:
        TransactionFilter() = default;

        void setPositionId(PositionId positionId);
        [[nodiscard]] std::optional<PositionId> getPositionId() const;

        [[nodiscard]]
        filter::Predicate<Transaction> getPredicate() const;
    };

    filter::Predicate<Transaction> HasPositionId(PositionId positionId);
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_FILTER_HPP__