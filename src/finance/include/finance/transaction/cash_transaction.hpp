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
        /// The amount of the cash transaction
        Cash _amount;

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

        [[nodiscard]] Cash getAmount() const;

        [[nodiscard]]
        TransactionEntries getEntries(AccountId externalAccount) const override;

       private:
        [[nodiscard]] TransactionEntry _getAmountEntry(
            std::optional<AccountId> external = std::nullopt
        ) const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__CASH_TRANSACTION_HPP__
