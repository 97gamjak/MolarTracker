#include "finance/transaction/transaction_filter.hpp"

#include <sstream>

#include "finance/transaction/domain_transaction.hpp"

namespace finance
{
    /**
     * @brief Set the position ID to filter transactions by
     *
     * @param positionId The position ID to filter by
     */
    void TransactionFilter::setPositionIds(const idSet<PositionId>& positionIds)
    {
        _positionIds = positionIds;
    }

    idSet<PositionId> TransactionFilter::getPositionIds() const
    {
        return _positionIds;
    }

    /**
     * @brief Get a predicate function that can be used to filter transactions
     * based on the criteria set in this filter, this function generates a
     * predicate that can be applied to a collection of transactions to filter
     * them according to the position ID specified in this filter. If no
     * position ID is set, the returned predicate will not filter out any
     * transactions based on position ID.
     *
     * @return filter::Predicate<DomainTransaction> A predicate function that
     * can be used to filter transactions based on the criteria set in this
     * filter.
     */
    filter::Predicate<DomainTransaction> TransactionFilter::getPredicate() const
    {
        if (!_positionIds.empty())
            return HasPositionId(_positionIds);

        return {};
    }

    filter::Predicate<DomainTransaction> HasPositionId(
        const idSet<PositionId>& positionIds
    )
    {
        return filter::makePredicate<DomainTransaction>(
            [positionIds](const DomainTransaction& transaction)
            {
                for (const auto& positionId : positionIds)
                {
                    if (hasId(
                            transaction.getData(),
                            positionId,
                            &TradeLeg::getPositionId
                        ))
                        return true;
                }
                return false;
            }
        );
    }

    std::string TransactionFilter::toString() const
    {
        std::ostringstream oss;
        oss << "TransactionFilter: [";
        for (const auto& positionId : _positionIds)
            oss << positionId << ", ";

        oss << "]";
        return oss.str();
    }

}   // namespace finance