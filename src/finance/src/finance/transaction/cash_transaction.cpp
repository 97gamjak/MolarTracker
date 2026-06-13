#include "finance/transaction/cash_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "utils/cash.hpp"

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
        : Transaction(
              id,
              timestamp,
              status,
              cashAccount,
              externalAccount,
              fees,
              std::move(comment)
          ),
          _amount(amount)
    {
    }

    /**
     * @brief Get the amount of the cash transaction
     *
     * @return Cash
     */
    Cash CashTransaction::getAmount() const { return _amount; }

    /**
     * @brief Get the transaction entries associated with the cash transaction
     *
     * @return TransactionEntries
     */
    TransactionEntries CashTransaction::getEntries(
        AccountId externalAccount
    ) const
    {
        TransactionEntries entries;

        entries.add(_getAmountEntry());
        entries.add(_getAmountEntry(externalAccount));
        entries.add(_getFeeEntry());
        entries.add(_getFeeEntry(externalAccount));

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
    TransactionEntry CashTransaction::_getAmountEntry(
        std::optional<AccountId> external
    ) const
    {
        const auto accountId =
            external.has_value() ? external.value() : getCashAccountId();

        const auto amount = external.has_value() ? -_amount : _amount;

        return TransactionEntry{
            TransactionEntryId::invalid(),
            accountId,
            amount,
            TransactionEntryType::General
        };
    }

}   // namespace finance
