#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_ENTRIES_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_ENTRIES_HPP__

#include <vector>

#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    class TransactionEntries
    {
       private:
        std::vector<TransactionEntry> _entries;

       public:
        TransactionEntries() = default;
        explicit TransactionEntries(
            const std::vector<TransactionEntry>& entries
        );

        void addTransactionEntry(const TransactionEntry& entry);

        [[nodiscard]]
        TransactionEntries filter(TransactionEntryType type) const;

        [[nodiscard]]
        TransactionEntries filter(const idSet<AccountId>& accountIds) const;

        [[nodiscard]] auto begin() const { return _entries.begin(); }
        [[nodiscard]] auto end() const { return _entries.end(); }
        [[nodiscard]] auto size() const { return _entries.size(); }
        [[nodiscard]] auto empty() const { return _entries.empty(); }
        [[nodiscard]] auto front() const { return _entries.front(); }
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_ENTRIES_HPP__
