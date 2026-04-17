#include "drafts/account_mapper.hpp"

#include "config/id_types.hpp"
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
            .status   = account.getStatus(),
        };
    }

    finance::Account AccountMapper::toAccount(const AccountDraft& draft)
    {
        return finance::Account{
            draft.id,
            draft.status.value_or(AccountStatus::Active),
            draft.name,
            draft.currency,
            draft.kind
        };
    }

}   // namespace drafts
