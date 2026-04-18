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
     * @param accountKind_
     */
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId     accountId_,
        finance::Cash cash_,
        AccountKind   accountKind_
    )
        : accountId(accountId_), cash(cash_), accountKind(accountKind_)
    {
    }
}   // namespace drafts