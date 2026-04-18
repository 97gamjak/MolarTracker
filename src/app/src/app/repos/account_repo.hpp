#ifndef __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__
#define __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__

#include "app/repos/base_repo.hpp"
#include "app/repos_api/i_account_repo.hpp"

namespace app
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

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__