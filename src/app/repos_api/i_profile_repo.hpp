#ifndef __APP__REPOS__PROFILE_REPO_HPP__
#define __APP__REPOS__PROFILE_REPO_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "domain/profile.hpp"
#include "sql_models/profile_row.hpp"

namespace app
{

    class Profile;   // forward declaration

    /**
     * @brief Interface for Profile repository
     *
     */
    class IProfileRepo
    {
       public:
        virtual ~IProfileRepo() = default;

        /**
         * @brief Ensure that the necessary database schema for the Profile
         * repository exists, this should be called before using any other
         * methods of the repository to ensure that the database is properly set
         * up to store and manage Profile data.
         *
         */
        virtual void ensureSchema() = 0;

        /**
         * @brief Get all profiles from the repository
         *
         * @return std::vector<Profile>
         */
        virtual std::vector<Profile> getAll() const = 0;

        /**
         * @brief Get a profile by its ID
         *
         * @param id The ID of the profile to retrieve
         * @return std::optional<Profile> The profile with the given ID, or
         * std::nullopt if no such profile exists
         */
        virtual std::optional<Profile> get(ProfileId id) const = 0;

        /**
         * @brief Get a profile by its name
         *
         * @param name The name of the profile to retrieve
         * @return std::optional<Profile> The profile with the given name, or
         * std::nullopt if no such profile exists
         */
        virtual std::optional<Profile> get(const std::string& name) const = 0;
        /**
         * @brief Create a new profile with the given name and email, this will
         * return the ID of the newly created profile which can be used to
         * retrieve it later. If a profile with the same name already exists, it
         * should throw an exception or return an error to indicate that the
         * profile could not be created.
         *
         * @param name The name of the profile to create
         * @param email The email of the profile to create (optional)
         * @return ProfileId The ID of the newly created profile
         */
        virtual ProfileId create(
            const std::string&         name,
            std::optional<std::string> email
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
         * @brief Remove the profile with the given ID from the repository, if
         * no profile with the given ID exists, it should throw an exception or
         * return an error to indicate that the removal failed.
         *
         * @param id The ID of the profile to remove
         */
        virtual void remove(ProfileId id) = 0;
    };

}   // namespace app

#endif   // __APP__REPOS__PROFILE_REPO_HPP__