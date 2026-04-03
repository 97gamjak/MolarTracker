#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__

#include <variant>

#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "finance/cash_transaction.hpp"

namespace finance
{
    using TransactionDetail = std::variant<CashTransaction>;

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

    /**
     * @brief Visitor for extracting the amount from a TransactionDetail.
     *
     */
    struct AmountVisitor
    {
        /**
         * @brief Extracts the amount from a TransactionDetail.
         *
         * @param detail The TransactionDetail to extract the amount from.
         * @return The extracted amount in micro_units.
         */
        micro_units operator()(const CashTransaction& detail) const
        {
            return detail.getAmount();
        }
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION_ENTRY_HPP__
