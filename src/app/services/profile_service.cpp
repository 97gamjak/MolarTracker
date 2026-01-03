#include "profile_service.hpp"

#include <optional>
#include <vector>

#include "domain/profile.hpp"
#include "repos/profile_repo.hpp"

namespace app
{

    ProfileService::ProfileService(IProfileRepo& profileRepo)
        : _profileRepo{profileRepo}
    {
    }

    std::optional<Profile> ProfileService::getProfile(ProfileID id) const
    {
        return _profileRepo.getByID(id);
    }

    std::vector<Profile> ProfileService::getAllProfiles() const
    {
        return _profileRepo.getAll();
    }

    ProfileID ProfileService::createProfile(const std::string& name)
    {
        return _profileRepo.create(name);
    }

    void ProfileService::renameProfile(ProfileID id, const std::string& newName)
    {
        _profileRepo.rename(id, newName);
    }

    void ProfileService::deleteProfile(ProfileID id)
    {
        _profileRepo.remove(id);
    }

    ProfileID ProfileService::ensureDefaultProfile(
        const std::string& defaultName
    )
    {
        const auto profiles = _profileRepo.getAll();
        if (!profiles.empty())
            return profiles.front().id();

        return _profileRepo.create(defaultName);
    }

}   // namespace app