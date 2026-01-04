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

    std::optional<Profile> ProfileService::getProfile(ProfileId id) const
    {
        return _profileRepo.getById(id);
    }

    std::vector<Profile> ProfileService::getAllProfiles() const
    {
        return _profileRepo.getAll();
    }

    ProfileId ProfileService::createProfile(
        const std::string& name,
        const std::optional<std::string>& /*email*/   // TODO: implement email
                                                      // support
    )
    {
        return _profileRepo.create(name);
    }

    void ProfileService::renameProfile(ProfileId id, const std::string& newName)
    {
        _profileRepo.rename(id, newName);
    }

    void ProfileService::deleteProfile(ProfileId id)
    {
        _profileRepo.remove(id);
    }

    bool ProfileService::profileExists(const std::string& name) const
    {
        return _profileRepo.getByName(name).has_value();
    }

}   // namespace app