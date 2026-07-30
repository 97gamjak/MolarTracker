#include "finance/transaction/transaction_filter.hpp"

#include <algorithm>
#include <sstream>

#include "config/strong_id.hpp"
#include "finance/transaction/domain_transaction.hpp"

namespace finance
{
    namespace
    {
        /**
         * @brief Get a predicate function that can be used to filter
         * transactions based on the specified position IDs.
         *
         * @param positionIds The set of position IDs to filter by
         * @return filter::Predicate<DomainTransaction> A predicate function
         * that can be used to filter transactions based on the specified
         * position IDs
         */
        filter::Predicate<DomainTransaction> HasPositionId(
            const IdSet<PositionId>& positionIds
        )
        {
            return filter::makePredicate<DomainTransaction>(
                [positionIds](const DomainTransaction& transaction)
                {
                    return std::ranges::any_of(
                        positionIds,
                        [&](const auto& positionId)
                        { return transaction.hasPositionId(positionId); }
                    );
                }
            );
        }

        /**
         * @brief Get a predicate function that can be used to filter
         * transactions based on the specified transaction IDs.
         *
         * @param transactionIds
         * @return filter::Predicate<DomainTransaction>
         */
        filter::Predicate<DomainTransaction> HasTransactionId(
            const IdSet<TransactionId>& transactionIds
        )
        {
            return filter::makePredicate<DomainTransaction>(
                [transactionIds](const DomainTransaction& transaction)
                { return transactionIds.contains(transaction.getId()); }
            );
        }

        filter::Predicate<DomainTransaction> HasAccountId(
            const IdSet<AccountId>& accountIds
        )
        {
            return filter::makePredicate<DomainTransaction>(
                [accountIds](const DomainTransaction& transaction)
                {
                    return std::ranges::any_of(
                        accountIds,
                        [&](const auto& accountId)
                        { return transaction.isAccountInvolved(accountId); }
                    );
                }
            );
        }
    }   // namespace

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
        if (accountIds.empty())
        {
            throw std::logic_error(
                "TransactionFilter must have at least one account ID set"
            );
        }

        filter::Predicate<DomainTransaction> predicate =
            HasAccountId(accountIds);

        if (!positionIds.empty())
            predicate &= HasPositionId(positionIds);

        if (!transactionIds.empty())
            predicate &= HasTransactionId(transactionIds);

        return predicate;
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
        for (const auto& positionId : positionIds)
            oss << positionId << ", ";
        for (const auto& transactionId : transactionIds)
            oss << transactionId << ", ";
        for (const auto& accountId : accountIds)
            oss << accountId << ", ";

        oss << "]";
        return oss.str();
    }

}   // namespace finance