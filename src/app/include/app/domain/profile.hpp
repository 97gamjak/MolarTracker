#ifndef __APP__INCLUDE__APP__DOMAIN__PROFILE_HPP__
#define __APP__INCLUDE__APP__DOMAIN__PROFILE_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "filter/predicate.hpp"

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
            std::string                       name,
            const std::optional<std::string>& email
        );

        [[nodiscard]] ProfileId                         getId() const;
        [[nodiscard]] const std::string&                getName() const;
        [[nodiscard]] const std::optional<std::string>& getEmail() const;

        void setId(ProfileId newId);
        void setName(const std::string& newName);
        void setEmail(const std::optional<std::string>& newEmail);
    };

    filter::Predicate<Profile> HasProfileId(ProfileId id);
    filter::Predicate<Profile> HasProfileName(std::string_view name);

}   // namespace app

#endif   // __APP__INCLUDE__APP__DOMAIN__PROFILE_HPP__