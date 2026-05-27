#include "account_mapper.hpp"

#include "config/id_types.hpp"
#include "drafts/account/account_draft.hpp"
#include "logic/finance/account.hpp"

namespace controller
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
    drafts::AccountDraft AccountMapper::toDraft(const finance::Account& account)
    {
        return drafts::AccountDraft{
            account.getId(),
            account.getName(),
            account.getKind(),
            account.getCurrency(),
            account.getStatus()
        };
    }

    /**
     * @brief Convert a vector of finance::Account to a vector of AccountDraft
     *
     * @param accounts The vector of finance::Account to convert
     * @return std::vector<drafts::AccountDraft> The vector of corresponding
     * AccountDraft
     */
    std::vector<drafts::AccountDraft> AccountMapper::toDrafts(
        const std::vector<finance::Account>& accounts
    )
    {
        std::vector<drafts::AccountDraft> drafts;
        drafts.reserve(accounts.size());
        for (const auto& account : accounts)
            drafts.push_back(toDraft(account));

        return drafts;
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
    finance::Account AccountMapper::fromDraft(const drafts::AccountDraft& draft)
    {
        auto account = finance::Account{
            AccountId::invalid(),
            draft.status.value_or(AccountStatus::Active),
            draft.name,
            draft.currency,
            draft.kind
        };
        account.setId(draft.id);
        return account;
    }

}   // namespace controller
