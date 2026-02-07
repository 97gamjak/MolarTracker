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
        ProfileId                  _id;
        std::string                _name;
        std::optional<std::string> _email;

       public:
        explicit Profile(
            ProfileId                         id,
            const std::string&                name,
            const std::optional<std::string>& email
        );

        //
        // getter and setter methods
        //

        // clang-format off
        ProfileId                        getId() const noexcept { return _id; }
        const std::string                getName() const noexcept { return _name; }
        const std::optional<std::string> getEmail() const noexcept { return _email; }

        void setId(ProfileId newId) { _id = newId; }
        void setName(const std::string& newName) { _name = newName; }
        void setEmail(const std::optional<std::string>& newEmail) { _email = newEmail; }
        // clang-format on
    };

}   // namespace app

#endif   // __APP__DOMAIN__PROFILE_HPP__