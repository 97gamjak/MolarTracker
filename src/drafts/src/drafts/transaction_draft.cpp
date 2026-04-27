#include "drafts/transaction_draft.hpp"

#include "config/finance.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Transaction Entry Draft:: Transaction Entry Draft
     * object
     *
     * @param accountId_
     * @param cash_
     */
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId     accountId_,
        finance::Cash cash_
    )
        : accountId(accountId_), cash(cash_)
    {
    }
}   // namespace drafts