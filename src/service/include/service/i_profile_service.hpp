#ifndef __SERVICE__INCLUDE__SERVICE__I_PROFILE_SERVICE_HPP__
#define __SERVICE__INCLUDE__SERVICE__I_PROFILE_SERVICE_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"

namespace domain
{
    class Profile;   // forward declaration
}   // namespace domain

namespace service
{

    /**
     * @brief Interface for Profile service
     *
     */
    class IProfileService
    {
       public:
        virtual ~IProfileService() = default;

        /**
         * @brief Get a profile by its ID
         *
         * @param id The ID of the profile to retrieve
         * @return std::optional<domain::Profile> The profile with the given ID,
         * or std::nullopt if no such profile exists
         */
        [[nodiscard]] virtual std::optional<domain::Profile> get(
            ProfileId id
        ) const = 0;

        /**
         * @brief Get all profiles
         *
         * @return std::vector<domain::Profile> A vector of all profiles
         */
        [[nodiscard]] virtual std::vector<domain::Profile> getAll() const = 0;

        /**
         * @brief Create a new profile with the given name and email, this will
         * return the ID of the newly created profile which can be used to
         * retrieve or modify the profile later on.
         *
         * @param profile The profile to create
         * @return ProfileId The ID of the newly created profile
         */
        [[nodiscard]] virtual ProfileId create(
            const domain::Profile& profile
        ) = 0;

        /**
         * @brief Update an existing profile with the given ID, changing its
         * name and email to the new values provided. If no profile with the
         * given ID exists, it should throw an exception or return an error to
         * indicate that the update failed.
         *
         * @param id The ID of the profile to update
         * @param newName The new name for the profile
         * @param newEmail The new email for the profile (optional)
         */
        virtual void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) = 0;

        /**
         * @brief Remove a profile by its ID, if no profile with the given ID
         * exists, it should throw an exception or return an error to indicate
         * that the removal failed.
         *
         * @param id The ID of the profile to remove
         */
        virtual void remove(ProfileId id) = 0;
    };

}   // namespace service

#endif   // __SERVICE__INCLUDE__SERVICE__I_PROFILE_SERVICE_HPP__