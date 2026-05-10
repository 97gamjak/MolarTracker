#include "finance/transaction_filter.hpp"

#include "finance/transaction.hpp"

namespace finance
{
    void TransactionFilter::setPositionId(PositionId positionId)
    {
        _positionId = positionId;
    }

    std::optional<PositionId> TransactionFilter::getPositionId() const
    {
        return _positionId;
    }

    filter::Predicate<Transaction> TransactionFilter::getPredicate() const
    {
        if (_positionId.has_value())
            return HasPositionId(_positionId.value());

        return {};
    }

    filter::Predicate<Transaction> HasPositionId(PositionId positionId)
    {
        return filter::makePredicate<Transaction>(
            [positionId](const Transaction& transaction)
            {
                return hasId(
                    transaction.getData(),
                    positionId,
                    &TradeLeg::getPositionId
                );
            }
        );
    }

}   // namespace finance