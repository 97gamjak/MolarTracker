#ifndef __REPO__SRC__REPO__ACCOUNT_REPO_HPP__
#define __REPO__SRC__REPO__ACCOUNT_REPO_HPP__

#include "base_repo.hpp"
#include "repo/i_account_repo.hpp"

namespace repo
{

    /**
     * @brief Database implementation of Account repository
     *
     */
    class AccountRepo : public IAccountRepo, public BaseRepo
    {
       public:
        using BaseRepo::BaseRepo;

        [[nodiscard]] std::vector<finance::Account> getAllAccounts(
            const ProfileId& profileId
        ) override;

        [[nodiscard]] AccountId createAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) override;
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__ACCOUNT_REPO_HPP__