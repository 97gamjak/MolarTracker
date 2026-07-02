#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_ENTRIES_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_ENTRIES_HPP__

#include "finance/transaction/transaction_entry.hpp"
#include "utils/container/set.hpp"
#include "utils/container/vector.hpp"

namespace finance
{
    /**
     * @brief Class representing a collection of transaction entries, providing
     * methods for filtering entries by type or associated account IDs.
     *
     */
    class TransactionEntries : public Vector<TransactionEntry>
    {
       public:
        using Vector<TransactionEntry>::Vector;

        [[nodiscard]]
        TransactionEntries filter(TransactionEntryType type) const;

        [[nodiscard]]
        TransactionEntries filter(const IdSet<AccountId>& accountIds) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_ENTRIES_HPP__
