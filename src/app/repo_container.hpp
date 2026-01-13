#ifndef __APP__REPO_CONTAINER_HPP__
#define __APP__REPO_CONTAINER_HPP__

#include "repos/profile_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

namespace app
{

    class RepoContainer
    {
       private:
        ProfileRepo _profileRepo;

       public:
        explicit RepoContainer(db::Database& db);

        IProfileRepo&       profileRepo();
        const IProfileRepo& profileRepo() const;
    };

}   // namespace app

#endif   // __APP__REPO_CONTAINER_HPP__