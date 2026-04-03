#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__

#include <variant>

#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/cash_transaction.hpp"

namespace finance
{
    using TransactionDetail = std::variant<CashTransaction>;

    class TransactionEntry
    {
       private:
        /// The unique identifier for the cash transaction entry
        TransactionEntryId _id;

        /// The account associated with the cash transaction
        AccountId _accountId;

        /// The cash amount involved in the transaction
        TransactionDetail _details;

       public:
        TransactionEntry(
            TransactionEntryId            id,
            AccountId                     accountId,
            std::variant<CashTransaction> details
        );

        [[nodiscard]] TransactionEntryId            getId() const;
        [[nodiscard]] AccountId                     getAccountId() const;
        [[nodiscard]] std::variant<CashTransaction> getDetails() const;
    };

    struct AmountVisitor
    {
        micro_units operator()(const CashTransaction& transaction) const
        {
            return transaction.getAmount();
        }
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__
