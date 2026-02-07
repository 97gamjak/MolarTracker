#include "app/repo_container.hpp"

#include "db/database.hpp"

namespace app
{

    /**
     * @brief Construct a new Repo Container object
     *
     * @param database
     */
    RepoContainer::RepoContainer(db::Database& database)
        : _profileRepo{database}
    {
    }

    /**
     * @brief Get the Profile Repo
     *
     * @return IProfileRepo&
     */
    IProfileRepo& RepoContainer::getProfileRepo() { return _profileRepo; }

    /**
     * @brief Get the Profile Repo (const version)
     *
     * @return const IProfileRepo&
     */
    const IProfileRepo& RepoContainer::getProfileRepo() const
    {
        return _profileRepo;
    }

}   // namespace app