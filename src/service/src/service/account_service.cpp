#include "account_service.hpp"

#include "finance/account/account.hpp"
#include "repo/i_account_repo.hpp"

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
        return _accountRepo->getAllAccounts(profileId);
    }

    /**
     * @brief Create a new account
     *
     * @param account The Account domain object containing the
     * details of the account to be created
     * @param profileId The ID of the profile to which the account belongs
     *
     * @return CrudResult<AccountId>
     */
    CrudResult<AccountId> AccountService::createAccount(
        const finance::Account& account,
        const ProfileId&        profileId
    )
    {
        const auto linkedSecurityAccountId =
            account.getLinkedSecurityAccountId();
        if (linkedSecurityAccountId.has_value())
        {
            if (!_accountRepo->accountExists(linkedSecurityAccountId.value()))
            {
                return CrudError{
                    CrudErrorType::NotFound,
                    "When creating account with name '" + account.getName() +
                        "' for profile with ID '" + profileId.toString() +
                        "', linked security account with ID '" +
                        linkedSecurityAccountId.value().toString() +
                        "' does not exist"
                };
            };
        }

        return _accountRepo->createAccount(account, profileId);
    }

}   // namespace service
