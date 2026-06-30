#include "finance/transaction/transaction_entries.hpp"

namespace finance
{

    /**
     * @brief Filters the transaction entries by type.
     *
     * @param type The type to filter by.
     * @return TransactionEntries The filtered transaction entries.
     */
    TransactionEntries TransactionEntries::filter(
        TransactionEntryType type
    ) const
    {
        TransactionEntries filtered;
        for (const auto& entry : *this)
            if (entry.getType() == type)
                filtered.add(entry);

        return filtered;
    }

    /**
     * @brief Filters the transaction entries by account ID.
     *
     * @param accountIds The account IDs to filter by.
     * @return TransactionEntries The filtered transaction entries.
     */
    TransactionEntries TransactionEntries::filter(
        const IdSet<AccountId>& accountIds
    ) const
    {
        TransactionEntries filtered;
        for (const auto& entry : *this)
            if (accountIds.contains(entry.getAccountId()))
                filtered.add(entry);

        return filtered;
    }

}   // namespace finance
