#include "account_service.hpp"

#include "app/repos_api/i_account_repo.hpp"
#include "finance/account.hpp"

namespace app
{

    /**
     * @brief Construct a new Account Service:: Account Service object
     *
     * @param accountRepo
     */
    AccountService::AccountService(
        const std::shared_ptr<IAccountRepo>& accountRepo
    )
        : _accountRepo(accountRepo)
    {
    }

    /**
     * @brief Get all accounts
     *
     * @param profileId The ID of the profile whose accounts are to be retrieved
     *
     * @return std::vector<finance::AccountVariant>
     */
    std::vector<finance::Account> AccountService::getAllAccounts(
        const ProfileId& profileId
    ) const
    {
        const auto cashAccounts = _accountRepo->getAllCashAccounts(profileId);

        std::vector<finance::Account> accounts;
        accounts.reserve(cashAccounts.size());

        for (const auto& account : cashAccounts)
            accounts.emplace_back(account);

        return accounts;
    }

    /**
     * @brief Get all cash accounts
     *
     * @param profileId The ID of the profile whose cash accounts are to be
     * retrieved
     *
     * @return std::vector<finance::CashAccount>
     */
    std::vector<finance::Account> AccountService::getAllCashAccounts(
        const ProfileId& profileId
    ) const
    {
        return _accountRepo->getAllCashAccounts(profileId);
    }

    /**
     * @brief Create a new cash account
     *
     * @param cashAccount The CashAccount domain object containing the
     * details of the cash account to be created
     * @param profileId The ID of the profile to which the cash account belongs
     *
     * @return AccountId
     */
    AccountId AccountService::createCashAccount(
        const finance::Account& cashAccount,
        const ProfileId&        profileId
    )
    {
        return _accountRepo->createCashAccount(cashAccount, profileId);
    }

}   // namespace app