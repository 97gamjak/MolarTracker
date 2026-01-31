#include "profile_service.hpp"

#include <optional>
#include <vector>

#include "domain/profile.hpp"
#include "repos/profile_repo.hpp"

namespace app
{

    ProfileService::ProfileService(IProfileRepo& getProfileRepo)
        : _profileRepo{getProfileRepo}
    {
    }

    std::optional<Profile> ProfileService::get(ProfileId id) const
    {
        return _profileRepo.get(id);
    }

    std::vector<Profile> ProfileService::getAll() const
    {
        return _profileRepo.getAll();
    }

    ProfileId ProfileService::create(
        const std::string&                name,
        const std::optional<std::string>& email
    )
    {
        return _profileRepo.create(name, email);
    }

    void ProfileService::update(
        ProfileId                         id,
        const std::string&                newName,
        const std::optional<std::string>& newEmail
    )
    {
        _profileRepo.update(id, newName, newEmail);
    }

    void ProfileService::remove(ProfileId id) { _profileRepo.remove(id); }

}   // namespace app