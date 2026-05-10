#include "finance/transaction_filter.hpp"

#include "finance/transaction.hpp"

namespace finance
{
    /**
     * @brief Set the position ID to filter transactions by
     *
     * @param positionId The position ID to filter by
     */
    void TransactionFilter::setPositionId(PositionId positionId)
    {
        _positionId = positionId;
    }

    /**
     * @brief Get the position ID that this filter is filtering by, if any
     *
     * @return std::optional<PositionId> The position ID that this filter is
     * filtering by, or std::nullopt if no position ID is set
     */
    std::optional<PositionId> TransactionFilter::getPositionId() const
    {
        return _positionId;
    }

    /**
     * @brief Get a predicate function that can be used to filter transactions
     * based on the criteria set in this filter, this function generates a
     * predicate that can be applied to a collection of transactions to filter
     * them according to the position ID specified in this filter. If no
     * position ID is set, the returned predicate will not filter out any
     * transactions based on position ID.
     *
     * @return filter::Predicate<Transaction> A predicate function that can be
     * used to filter transactions based on the criteria set in this filter.
     */
    filter::Predicate<Transaction> TransactionFilter::getPredicate() const
    {
        if (_positionId.has_value())
            return HasPositionId(_positionId.value());

        return {};
    }

    /**
     * @brief Create a predicate function to filter transactions by a specific
     * position ID, this function generates a predicate that can be applied to a
     * collection of transactions to filter them based on whether they are
     * associated with the specified position ID. The predicate checks if any of
     * the trade legs in the transaction's data have a position ID that matches
     * the given position ID.
     *
     * @param positionId The position ID to filter transactions by, this
     * specifies the position that the returned predicate will filter
     * transactions for.
     *
     * @return filter::Predicate<Transaction> A predicate function that can be
     * used to filter transactions based on whether they are associated with the
     * specified position ID.
     */
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