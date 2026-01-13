#include "app/repo_container.hpp"

#include "db/database.hpp"

namespace app
{

    RepoContainer::RepoContainer(db::Database& database)
        : _profileRepo{database}
    {
    }

    IProfileRepo& RepoContainer::profileRepo() { return _profileRepo; }

    const IProfileRepo& RepoContainer::profileRepo() const
    {
        return _profileRepo;
    }

}   // namespace app