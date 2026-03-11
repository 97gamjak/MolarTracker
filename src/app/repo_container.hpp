#ifndef __APP__REPO_CONTAINER_HPP__
#define __APP__REPO_CONTAINER_HPP__

#include <memory>

#include "repos_api/i_profile_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    class IProfileRepo;   // Forward declaration
    class IAccountRepo;   // Forward declaration

    /**
     * @brief Container for all repositories
     *
     */
    class RepoContainer
    {
       private:
        /// The Profile repository
        std::shared_ptr<IProfileRepo> _profileRepo;
        /// The Account repository
        std::shared_ptr<IAccountRepo> _accountRepo;

       public:
        explicit RepoContainer(const std::shared_ptr<db::Database>& db);

        [[nodiscard]] std::shared_ptr<IProfileRepo>       getProfileRepo();
        [[nodiscard]] std::shared_ptr<const IProfileRepo> getProfileRepo(
        ) const;

        [[nodiscard]] std::shared_ptr<IAccountRepo>       getAccountRepo();
        [[nodiscard]] std::shared_ptr<const IAccountRepo> getAccountRepo(
        ) const;
    };

}   // namespace app

#endif   // __APP__REPO_CONTAINER_HPP__