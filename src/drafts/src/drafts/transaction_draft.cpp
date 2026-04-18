#include "drafts/transaction_draft.hpp"

#include "config/finance.hpp"

namespace drafts
{
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId     accountId,
        finance::Cash cash,
        AccountKind   accountKind
    )
        : accountId(accountId), cash(cash), accountKind(accountKind)
    {
    }
}   // namespace drafts