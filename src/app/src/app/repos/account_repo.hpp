#ifndef __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__
#define __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__

#include "app/repos_api/i_account_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    /**
     * @brief Database implementation of Account repository
     *
     */
    class AccountRepo : public IAccountRepo
    {
       private:
        /// reference to the database instance
        db::Database& _db;

       public:
        explicit AccountRepo(db::Database& db);

        ~AccountRepo() override                    = default;
        AccountRepo(const AccountRepo&)            = delete;
        AccountRepo& operator=(const AccountRepo&) = delete;
        AccountRepo(AccountRepo&&)                 = delete;
        AccountRepo& operator=(AccountRepo&&)      = delete;

        [[nodiscard]] std::vector<finance::Account> getAllCashAccounts(
            const ProfileId& profileId
        ) const override;

        [[nodiscard]] AccountId createCashAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) override;
    };

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__