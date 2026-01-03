#ifndef __APP__REPO_CONTAINER_HPP__
#define __APP__REPO_CONTAINER_HPP__

#include "profile_repo.hpp"

namespace db
{
    class Database;   // Forward declaration
}   // namespace db

class RepoContainer
{
   private:
    ProfileRepo _profileRepo;

   public:
    explicit RepoContainer(db::Database& db);

    IProfileRepo&       profileRepo() noexcept;
    const IProfileRepo& profileRepo() const noexcept;
};

#endif   // __APP__REPO_CONTAINER_HPP__