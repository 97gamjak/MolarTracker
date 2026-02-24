#ifndef __FINANCE__CASH_TRANSACTION_HPP__
#define __FINANCE__CASH_TRANSACTION_HPP__

#include "cash.hpp"
#include "config/id_types.hpp"

namespace finance
{
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