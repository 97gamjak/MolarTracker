#include "drafts/account_mapper.hpp"

#include "drafts/account_draft.hpp"
#include "finance/account.hpp"

namespace drafts
{

    AccountDraft AccountMapper::toDraft(const finance::Account& account)
    {
        return AccountDraft{
            .id       = account.getId(),
            .name     = account.getName(),
            .kind     = account.getKind(),
            .currency = account.getCurrency(),
        };
    }

    finance::Account AccountMapper::toAccount(const AccountDraft& draft)
    {
        return finance::Account{
            draft.id,
            draft.name,
            draft.kind,
            draft.currency
        };
    }

}   // namespace drafts
