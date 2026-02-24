#include "cash_transaction.hpp"

namespace finance
{
    /**
     * @brief Construct a new Cash Transaction:: Cash Transaction object
     *
     * @param id
     * @param accountId
     * @param cash
     */
    CashTransaction::CashTransaction(
        TransactionEntryId id,
        AccountId          accountId,
        Cash               cash
    )
        : _id(id), _accountId(accountId), _cash(cash)
    {
    }

}   // namespace finance