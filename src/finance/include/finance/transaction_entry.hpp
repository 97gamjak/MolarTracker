#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"

namespace finance
{
    /**
     * @brief Represents a single entry in a financial transaction.
     *
     */
    class TransactionEntry
    {
       private:
        /// The unique identifier for the cash transaction entry
        TransactionEntryId _id;

        /// The account associated with the cash transaction
        AccountId _accountId;

        Cash _cash;

       public:
        TransactionEntry(TransactionEntryId id, AccountId accountId, Cash cash);

        [[nodiscard]] TransactionEntryId getId() const;
        [[nodiscard]] AccountId          getAccountId() const;
        [[nodiscard]] micro_units        getAmount() const;
        [[nodiscard]] Currency           getCurrency() const;
        [[nodiscard]] Cash               getCash() const;

        void setAccountId(AccountId accountId);
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__
