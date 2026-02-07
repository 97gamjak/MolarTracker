#include "service_container.hpp"

#include "app/repo_container.hpp"

namespace app
{

    /**
     * @brief Construct a new Service Container object
     *
     * @param repos
     */
    ServiceContainer::ServiceContainer(RepoContainer& repos)
        : _profileService{repos.getProfileRepo()}
    {
    }

    /**
     * @brief Get the Profile Service
     *
     * @return IProfileService&
     */
    IProfileService& ServiceContainer::getProfileService()
    {
        return _profileService;
    }

    /**
     * @brief Get the Profile Service (const version)
     *
     * @return const IProfileService&
     */
    const IProfileService& ServiceContainer::getProfileService() const
    {
        return _profileService;
    }

}   // namespace app