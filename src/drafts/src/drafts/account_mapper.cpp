#include "drafts/account_mapper.hpp"

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
            account.getStatus(),
            account.getName(),
            account.getCurrency(),
            account.getKind()
        };
    }

    /**
     * @brief Convert a vector of finance::Account to a vector of AccountDraft
     *
     * @param accounts The vector of finance::Account to convert, these are the
     * domain model representations of accounts, and contain all the details of
     * the accounts as they exist in the business logic layer.
     * @return std::vector<AccountDraft> A vector of corresponding
     * AccountDrafts, these are the draft model representations of accounts, and
     * are used for transferring data between the business logic and the UI,
     * they contain the same information as the finance::Account but may be
     * structured differently to better suit the needs of the UI.
     */
    std::vector<AccountDraft> AccountMapper::toDrafts(
        const std::vector<finance::Account>& accounts
    )
    {
        std::vector<AccountDraft> drafts;
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
    finance::Account AccountMapper::toAccount(const AccountDraft& draft)
    {
        auto account = finance::Account{
            draft.getId(),
            draft.getStatus().value_or(AccountStatus::Active),
            draft.getName(),
            draft.getCurrency(),
            draft.getKind()
        };
        return account;
    }

}   // namespace drafts
