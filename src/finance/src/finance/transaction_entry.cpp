#include "finance/transaction_entry.hpp"

#include "config/id_types.hpp"

namespace finance
{
    /**
     * @brief Construct a new TransactionEntry:: TransactionEntry object
     *
     * @param id
     * @param accountId
     * @param details
     */
    TransactionEntry::TransactionEntry(
        TransactionEntryId id,
        AccountId          accountId,
        TransactionDetail  details
    )
        : _id(id), _accountId(accountId), _details(details)
    {
    }

    /**
     * @brief Gets the ID of the transaction entry.
     *
     * @return TransactionEntryId The ID of the transaction entry.
     */
    TransactionEntryId TransactionEntry::getId() const { return _id; }

    /**
     * @brief Gets the account ID associated with the transaction entry.
     *
     * @return AccountId The account ID associated with the transaction entry.
     */
    AccountId TransactionEntry::getAccountId() const { return _accountId; }

    /**
     * @brief Gets the details of the transaction entry.
     *
     * @return TransactionDetail The details of the transaction entry.
     */
    TransactionDetail TransactionEntry::getDetails() const { return _details; }
}   // namespace finance
