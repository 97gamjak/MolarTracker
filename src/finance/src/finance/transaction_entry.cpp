#include "finance/transaction_entry.hpp"

#include "config/id_types.hpp"

namespace finance
{
    /**
     * @brief Construct a new TransactionEntry:: TransactionEntry object
     *
     * @param id
     * @param accountId
     * @param cash
     */
    TransactionEntry::TransactionEntry(
        TransactionEntryId id,
        AccountId          accountId,
        Cash               cash
    )
        : _id(id), _accountId(accountId), _cash(cash)
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
     * @brief Gets the amount of the transaction entry.
     *
     * @return micro_units The amount of the transaction entry in micro units.
     */
    micro_units TransactionEntry::getAmount() const
    {
        return _cash.getAmount();
    }

    /**
     * @brief Gets the currency of the transaction entry.
     *
     * @return Currency The currency of the transaction entry.
     */
    Currency TransactionEntry::getCurrency() const
    {
        return _cash.getCurrency();
    }

    /**
     * @brief Gets the cash associated with the transaction entry.
     *
     * @return Cash The cash associated with the transaction entry.
     */
    [[nodiscard]] Cash TransactionEntry::getCash() const { return _cash; }

    /**
     * @brief Sets the account ID associated with the transaction entry.
     *
     * @param accountId The account ID to associate with the transaction entry.
     */
    void TransactionEntry::setAccountId(AccountId accountId)
    {
        _accountId = accountId;
    }
}   // namespace finance
