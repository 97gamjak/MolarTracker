#include "account_service.hpp"

#include "finance/account/account.hpp"
#include "logging/log_macros.hpp"
#include "repo/i_account_repo.hpp"

REGISTER_LOG_CATEGORY("Service.AccountService");

namespace service
{

    /**
     * @brief Construct a new Account Service:: Account Service object
     *
     * @param accountRepo
     */
    AccountService::AccountService(
        const std::shared_ptr<repo::IAccountRepo>& accountRepo
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
        const auto& accounts = _accountRepo->getAllAccounts(profileId);

        LOG_DEBUG(
            std::format(
                "Retrieved {} accounts from repo for profile {}",
                accounts.size(),
                profileId.toString()
            )
        );

        return accounts;
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

}   // namespace service