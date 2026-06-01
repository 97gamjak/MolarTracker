#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__CASH_TRANSACTION_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__CASH_TRANSACTION_HPP__

#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/transaction/transaction.hpp"
#include "finance/transaction/transaction_entries.hpp"
#include "finance/transaction/transaction_entry.hpp"

namespace finance
{
    /**
     * @brief A cash transaction represents a financial transaction involving
     * cash.
     *
     */
    class CashTransaction : public Transaction
    {
       private:
        AccountId _cashAccount;

        AccountId _externalAccount;

        Cash _amount;

        Cash _fees;

       public:
        CashTransaction(
            TransactionId              id,
            Timestamp                  timestamp,
            TransactionStatus          status,
            AccountId                  cashAccount,
            AccountId                  externalAccount,
            Cash                       amount,
            Cash                       fees,
            std::optional<std::string> comment = std::nullopt
        );

        [[nodiscard]] AccountId getCashAccountId() const;
        [[nodiscard]] AccountId getExternalAccountId() const;
        [[nodiscard]] Cash      getAmount() const;
        [[nodiscard]] Cash      getFees() const;

        [[nodiscard]]
        TransactionEntries getTransactionEntries() const;

       private:
        [[nodiscard]] TransactionEntry _getAmountEntry(bool external) const;
        [[nodiscard]] TransactionEntry _getFeeEntry(bool external) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__CASH_TRANSACTION_HPP__
