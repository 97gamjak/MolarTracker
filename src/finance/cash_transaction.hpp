#ifndef __FINANCE__CASH_TRANSACTION_HPP__
#define __FINANCE__CASH_TRANSACTION_HPP__

#include "cash.hpp"
#include "config/id_types.hpp"

namespace finance
{
    /**
     * @brief A class representing a cash transaction entry, which is a specific
     * type of transaction entry that involves a cash movement in a specific
     * account. This class includes the account associated with the transaction
     * and the cash amount involved in the transaction. It serves as a component
     * of a larger financial transaction, which may include multiple entries
     * representing different cash movements or account changes related to the
     * transaction.
     *
     */
    class CashTransaction
    {
       private:
        /// The unique identifier for the cash transaction entry
        TransactionEntryId _id;

        /// The account associated with the cash transaction
        AccountId _accountId;

        /// The cash amount involved in the transaction
        Cash _cash;

       public:
        CashTransaction(TransactionEntryId id, AccountId accountId, Cash cash);
    };
}   // namespace finance

#endif   // __FINANCE__CASH_TRANSACTION_HPP__