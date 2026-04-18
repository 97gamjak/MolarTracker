#include "drafts/transaction_draft.hpp"

#include "config/finance.hpp"

namespace drafts
{
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId   accountId,
        micro_units amount,
        Currency    currency,
        AccountKind accountKind
    )
        : accountId(accountId),
          amount(amount),
          currency(currency),
          accountKind(accountKind)
    {
    }
}   // namespace drafts