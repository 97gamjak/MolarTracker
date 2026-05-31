#include "finance/transaction/transaction_entries.hpp"

namespace finance
{
    TransactionEntries::TransactionEntries(
        const std::vector<TransactionEntry>& entries
    )
        : _entries(entries)
    {
    }

    void TransactionEntries::addTransactionEntry(const TransactionEntry& entry)
    {
        _entries.push_back(entry);
    }

    TransactionEntries TransactionEntries::filter(
        TransactionEntryType type
    ) const
    {
        TransactionEntries filtered;
        for (const auto& entry : _entries)
            if (entry.getType() == type)
                filtered.addTransactionEntry(entry);

        return filtered;
    }

    TransactionEntries TransactionEntries::filter(
        const idSet<AccountId>& accountIds
    ) const
    {
        TransactionEntries filtered;
        for (const auto& entry : _entries)
            if (accountIds.contains(entry.getAccountId()))
                filtered.addTransactionEntry(entry);

        return filtered;
    }

}   // namespace finance
