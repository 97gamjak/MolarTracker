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

        ProfileId                        getId() const noexcept;
        const std::string                getName() const noexcept;
        const std::optional<std::string> getEmail() const noexcept;

        void setId(ProfileId newId);
        void setName(const std::string& newName);
        void setEmail(const std::optional<std::string>& newEmail);
    };

}   // namespace app

#endif   // __APP__DOMAIN__PROFILE_HPP__