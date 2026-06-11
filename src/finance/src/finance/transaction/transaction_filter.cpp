#include "finance/transaction/transaction_filter.hpp"

#include <sstream>

#include "config/strong_id.hpp"
#include "finance/transaction/domain_transaction.hpp"

namespace finance
{
    /**
     * @brief Set the position IDs used to filter transactions, this will set
     * the position IDs that will be used to filter transactions when querying
     * the transaction store. Transactions that are associated with any of the
     * specified position IDs will be included in the results, while
     * transactions that are not associated with any of the specified position
     * IDs will be excluded from the results.
     *
     * @param positionIds The set of position IDs to filter transactions by
     */
    void TransactionFilter::setPositionIds(const idSet<PositionId>& positionIds)
    {
        _positionIds = positionIds;
    }

    /**
     * @brief Get the position IDs used to filter transactions
     *
     * @return idSet<PositionId> The set of position IDs used for filtering
     */
    idSet<PositionId> TransactionFilter::getPositionIds() const
    {
        return _positionIds;
    }

    /**
     * @brief get the transaction IDs used to filter transactions
     *
     * @return idSet<TransactionId>
     */
    idSet<TransactionId> TransactionFilter::getTransactionIds() const
    {
        return _transactionIds;
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

        if (!_transactionIds.empty())
            return HasTransactionId(_transactionIds);

        return {};
    }

    /**
     * @brief Get a predicate function that can be used to filter transactions
     * based on the specified position IDs.
     *
     * @param positionIds The set of position IDs to filter by
     * @return filter::Predicate<DomainTransaction> A predicate function that
     * can be used to filter transactions based on the specified position IDs
     */
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

    /**
     * @brief Get a predicate function that can be used to filter transactions
     * based on the specified transaction IDs.
     *
     * @param transactionIds
     * @return filter::Predicate<DomainTransaction>
     */
    filter::Predicate<DomainTransaction> HasTransactionId(
        const idSet<TransactionId>& transactionIds
    )
    {
        return filter::makePredicate<DomainTransaction>(
            [transactionIds](const DomainTransaction& transaction)
            { return transactionIds.contains(transaction.getId()); }
        );
    }

    /**
     * @brief Get a string representation of the transaction filter, this is
     * used for logging and debugging purposes
     *
     * @return std::string
     */
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