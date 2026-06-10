#include "finance/transaction/cash_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/transaction_entries.hpp"

namespace finance
{
    /**
     * @brief Construct a new Cash Transaction:: Cash Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param cashAccount
     * @param externalAccount
     * @param amount
     * @param fees
     * @param comment
     */
    CashTransaction::CashTransaction(
        TransactionId              id,
        Timestamp                  timestamp,
        TransactionStatus          status,
        AccountId                  cashAccount,
        AccountId                  externalAccount,
        Cash                       amount,
        Cash                       fees,
        std::optional<std::string> comment
    )
        : Transaction(id, timestamp, status, std::move(comment)),
          _cashAccount(cashAccount),
          _externalAccount(externalAccount),
          _amount(amount),
          _fees(fees)
    {
    }

    /**
     * @brief Get the cash account ID associated with the cash transaction
     *
     * @return AccountId
     */
    AccountId CashTransaction::getCashAccountId() const { return _cashAccount; }

    /**
     * @brief Get the external account ID associated with the cash transaction
     *
     * @return AccountId
     */
    AccountId CashTransaction::getExternalAccountId() const
    {
        return _externalAccount;
    }

    /**
     * @brief Get the amount of the cash transaction
     *
     * @return Cash
     */
    Cash CashTransaction::getAmount() const { return _amount; }

    /**
     * @brief Get the fees associated with the cash transaction
     *
     * @return Cash
     */
    Cash CashTransaction::getFees() const { return _fees; }

    /**
     * @brief Get the transaction entries associated with the cash transaction
     *
     * @return TransactionEntries
     */
    TransactionEntries CashTransaction::getTransactionEntries() const
    {
        TransactionEntries entries;

        entries.add(_getAmountEntry(false));
        entries.add(_getAmountEntry(true));
        entries.add(_getFeeEntry(false));
        entries.add(_getFeeEntry(true));

        return entries;
    }

    /**
     * @brief Get the amount entry for the cash transaction, this will create a
     * transaction entry for the amount of the cash transaction, if the entry is
     * for the external account it will negate the amount to reflect the cash
     * flow correctly.
     *
     * @param external
     * @return TransactionEntry
     */
    TransactionEntry CashTransaction::_getAmountEntry(bool external) const
    {
        return TransactionEntry{
            TransactionEntryId::invalid(),
            external ? _externalAccount : _cashAccount,
            external ? -_amount : _amount,
            TransactionEntryType::General
        };
    }

    /**
     * @brief Get the fee entry for the cash transaction, this will create a
     * transaction entry for the fees of the cash transaction, if the entry is
     * for the external account it will negate the fees to reflect the cash
     * flow correctly.
     *
     * @param external
     * @return TransactionEntry
     */
    TransactionEntry CashTransaction::_getFeeEntry(bool external) const
    {
        return TransactionEntry{
            TransactionEntryId::invalid(),
            external ? _externalAccount : _cashAccount,
            external ? -_fees : _fees,
            TransactionEntryType::Fees
        };
    }

}   // namespace finance
