#include "profile_service.hpp"

#include <optional>
#include <vector>

#include "domain/profile.hpp"
#include "repos/profile_repo.hpp"

namespace app
{

    /**
     * @brief Construct a new Profile Service:: Profile Service object
     *
     * @param getProfileRepo
     */
    ProfileService::ProfileService(IProfileRepo& getProfileRepo)
        : _profileRepo{getProfileRepo}
    {
    }

    /**
     * @brief Get a profile by its ID
     *
     * @param id
     * @return std::optional<Profile>
     */
    std::optional<Profile> ProfileService::get(ProfileId id) const
    {
        return _profileRepo.get(id);
    }

    /**
     * @brief Get all profiles
     *
     * @return std::vector<Profile>
     */
    std::vector<Profile> ProfileService::getAll() const
    {
        return _profileRepo.getAll();
    }

    /**
     * @brief Create a new profile
     *
     * @param name
     * @param email
     * @return ProfileId
     */
    ProfileId ProfileService::create(
        const std::string&                name,
        const std::optional<std::string>& email
    )
    {
        return _profileRepo.create(name, email);
    }

    /**
     * @brief Update a profile's name and email
     *
     * @param id
     * @param newName
     * @param newEmail
     */
    void ProfileService::update(
        ProfileId                         id,
        const std::string&                newName,
        const std::optional<std::string>& newEmail
    )
    {
        _profileRepo.update(id, newName, newEmail);
    }

    /**
     * @brief Remove a profile by its ID
     *
     * @param id
     */
    void ProfileService::remove(ProfileId id) { _profileRepo.remove(id); }

}   // namespace app