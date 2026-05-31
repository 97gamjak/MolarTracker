#include "finance/transaction/cash_transaction.hpp"

#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/transaction_entries.hpp"

namespace finance
{
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

    AccountId CashTransaction::getCashAccountId() const { return _cashAccount; }

    AccountId CashTransaction::getExternalAccountId() const
    {
        return _externalAccount;
    }

    Cash CashTransaction::getAmount() const { return _amount; }

    Cash CashTransaction::getFees() const { return _fees; }

    TransactionEntries CashTransaction::getTransactionEntries() const
    {
        TransactionEntries entries;

        entries.addTransactionEntry(_getAmountEntry(false));
        entries.addTransactionEntry(_getAmountEntry(true));
        entries.addTransactionEntry(_getFeeEntry(false));
        entries.addTransactionEntry(_getFeeEntry(true));

        return entries;
    }

    TransactionEntry CashTransaction::_getAmountEntry(bool external) const
    {
        return TransactionEntry{
            TransactionEntryId::invalid(),
            external ? _externalAccount : _cashAccount,
            external ? -_amount : _amount,
            TransactionEntryType::General
        };
    }

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
