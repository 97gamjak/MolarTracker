#include "profile_service.hpp"

#include <optional>
#include <vector>

#include "domain/profile.hpp"
#include "repo/i_profile_repo.hpp"

namespace service
{

    /**
     * @brief Construct a new Profile Service:: Profile Service object
     *
     * @param repo
     */
    ProfileService::ProfileService(
        const std::shared_ptr<repo::IProfileRepo>& repo
    )
        : _profileRepo{repo}
    {
    }

    /**
     * @brief Get a profile by its ID
     *
     * @param id
     * @return std::optional<domain::Profile>
     */
    std::optional<domain::Profile> ProfileService::get(ProfileId id) const
    {
        return _profileRepo->get(id);
    }

    /**
     * @brief Get all profiles
     *
     * @return std::vector<domain::Profile>
     */
    std::vector<domain::Profile> ProfileService::getAll() const
    {
        return _profileRepo->getAll();
    }

    /**
     * @brief Create a new profile
     *
     * @param profile
     * @return CrudResult<ProfileId>
     */
    CrudResult<ProfileId> ProfileService::create(const domain::Profile& profile)
    {
        return _profileRepo->create(profile);
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
        _profileRepo->update(id, newName, newEmail);
    }

    /**
     * @brief Remove a profile by its ID
     *
     * @param id
     */
    void ProfileService::remove(ProfileId id) { _profileRepo->remove(id); }

}   // namespace service
