#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__

#include <optional>

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
        idSet<PositionId> _positionIds;

       public:
        TransactionFilter() = default;

        void addPositionId(PositionId positionId);
        void setPositionIds(const idSet<PositionId>& positionIds);
        [[nodiscard]] idSet<PositionId> getPositionIds() const;

        [[nodiscard]]
        filter::Predicate<DomainTransaction> getPredicate() const;

        [[nodiscard]]
        std::string toString() const;
    };

    filter::Predicate<DomainTransaction> HasPositionId(
        const idSet<PositionId>& positionIds
    );
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_FILTER_HPP__