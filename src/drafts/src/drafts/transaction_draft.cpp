#include "drafts/transaction_draft.hpp"

#include "config/finance.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Transaction Entry Draft:: Transaction Entry Draft
     * object
     *
     * @param accountId
     * @param cash
     * @param accountKind
     */
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId     accountId,
        finance::Cash cash,
        AccountKind   accountKind
    )
        : accountId(accountId), cash(cash), accountKind(accountKind)
    {
    }
}   // namespace drafts