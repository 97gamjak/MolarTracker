#include "service_container.hpp"

#include "repo_container.hpp"

ServiceContainer::ServiceContainer(RepoContainer& repos)
    : _profileService{repos.profileRepo()}
{
}

IProfileService& ServiceContainer::profileService() noexcept
{
    return _profileService;
}

const IProfileService& ServiceContainer::profileService() const noexcept
{
    return _profileService;
}