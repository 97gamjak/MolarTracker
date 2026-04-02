#include "finance/transaction_entry.hpp"

namespace finance
{
    TransactionEntry::TransactionEntry(
        TransactionEntryId            id,
        AccountId                     accountId,
        std::variant<CashTransaction> details
    )
        : _id(id), _accountId(accountId), _details(details)
    {
    }

    TransactionEntryId TransactionEntry::getId() const { return _id; }

    AccountId TransactionEntry::getAccountId() const { return _accountId; }

    std::variant<CashTransaction> TransactionEntry::getDetails() const
    {
        return _details;
    }
}   // namespace finance
