#include "drafts/account_mapper.hpp"

#include "config/id_types.hpp"
#include "drafts/account_draft.hpp"
#include "finance/account.hpp"

namespace drafts
{

    /**
     * @brief Convert a finance::Account to an AccountDraft
     *
     * @param account The finance::Account to convert, this is the domain model
     * representation of an account, and contains all the details of the account
     * as it exists in the business logic layer.
     * @return AccountDraft The corresponding AccountDraft, this is the draft
     * model representation of an account, and is used for transferring data
     * between the business logic and the UI, it contains the same information
     * as the finance::Account but may be structured differently to better suit
     * the needs of the UI.
     */
    AccountDraft AccountMapper::toDraft(const finance::Account& account)
    {
        return AccountDraft{
            account.getId(),
            account.getName(),
            account.getKind(),
            account.getCurrency(),
            account.getStatus()
        };
    }

    /**
     * @brief Convert an AccountDraft to a finance::Account
     *
     * @param draft The AccountDraft to convert, this is the draft model
     * representation of an account, and is used for transferring data between
     * the business logic and the UI, it contains the same information as the
     * finance::Account but may be structured differently to better suit the
     * needs of the UI.
     * @return finance::Account The corresponding finance::Account, this is the
     * domain model representation of an account, and contains all the details
     * of the account as it exists in the business logic layer.
     */
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
