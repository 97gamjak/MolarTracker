#include "service_container.hpp"

#include "app/repo_container.hpp"

namespace app
{

    ServiceContainer::ServiceContainer(RepoContainer& repos)
        : _profileService{repos.profileRepo()}
    {
    }

    IProfileService& ServiceContainer::profileService()
    {
        return _profileService;
    }

    const IProfileService& ServiceContainer::profileService() const
    {
        return _profileService;
    }

}   // namespace app