#ifndef __APP__DOMAIN__PROFILE_HPP__
#define __APP__DOMAIN__PROFILE_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"

namespace app
{

    /**
     * @brief Domain model for a user profile
     *
     */
    class Profile
    {
       private:
        /// The unique identifier for the profile
        ProfileId _id;

        /// The name of the profile
        std::string _name;

        /// The email associated with the profile (optional)
        std::optional<std::string> _email;

       public:
        explicit Profile(
            ProfileId                         id,
            const std::string&                name,
            const std::optional<std::string>& email
        );

        [[nodiscard]] ProfileId                         getId() const;
        [[nodiscard]] const std::string&                getName() const;
        [[nodiscard]] const std::optional<std::string>& getEmail() const;

        void setId(ProfileId newId);
        void setName(const std::string& newName);
        void setEmail(const std::optional<std::string>& newEmail);
    };

    /**
     * @brief Predicate for finding a profile by its ID
     *
     */
    struct HasProfileId
    {
        /// The ID to compare against
        ProfileId id;

        [[nodiscard]] bool operator()(const Profile& profile) const;
    };

    /**
     * @brief Predicate for finding a profile by its name
     *
     */
    struct HasProfileName
    {
        /// The name to compare against
        std::string_view name;

        [[nodiscard]] bool operator()(const Profile& profile) const;
    };

}   // namespace app

#endif   // __APP__DOMAIN__PROFILE_HPP__