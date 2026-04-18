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
        return _accountRepo->getAllAccounts(profileId);
    }

    /**
     * @brief Create a new account
     *
     * @param account The Account domain object containing the
     * details of the account to be created
     * @param profileId The ID of the profile to which the account belongs
     *
     * @return AccountId
     */
    AccountId AccountService::createAccount(
        const finance::Account& account,
        const ProfileId&        profileId
    )
    {
        return _accountRepo->createAccount(account, profileId);
    }

}   // namespace app