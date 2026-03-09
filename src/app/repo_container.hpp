#ifndef __APP__REPO_CONTAINER_HPP__
#define __APP__REPO_CONTAINER_HPP__

#include "repos/profile_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    /**
     * @brief Container for all repositories
     *
     */
    class RepoContainer
    {
       private:
        /// The Profile repository
        std::shared_ptr<IProfileRepo> _profileRepo;

       public:
        explicit RepoContainer(const std::shared_ptr<db::Database>& db);

        [[nodiscard]] std::shared_ptr<IProfileRepo>       getProfileRepo();
        [[nodiscard]] std::shared_ptr<const IProfileRepo> getProfileRepo(
        ) const;
    };

}   // namespace app

#endif   // __APP__REPO_CONTAINER_HPP__