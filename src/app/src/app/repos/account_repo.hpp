#ifndef __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__
#define __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__

#include <memory>

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
        std::shared_ptr<db::Database> _db;

       public:
        explicit AccountRepo(const std::shared_ptr<db::Database>& db);

        void ensureSchema() override;

        [[nodiscard]] std::vector<finance::CashAccount> getAllCashAccounts(
            const ProfileId& profileId
        ) const override;

        [[nodiscard]] AccountId createCashAccount(
            const finance::CashAccount& account,
            const ProfileId&            profileId
        ) override;

       private:
        void _ensureSchema() const;
    };

}   // namespace app

#endif   // __APP__SRC__APP__REPOS__ACCOUNT_REPO_HPP__