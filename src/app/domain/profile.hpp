#ifndef __APP__DOMAIN__PROFILE_HPP__
#define __APP__DOMAIN__PROFILE_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"

namespace app
{

    class Profile
    {
       private:
        ProfileID                  _id;
        std::string                _name;
        std::optional<std::string> _email;

       public:
        explicit Profile(
            ProfileID                         id,
            const std::string&                name,
            const std::optional<std::string>& email
        );

        ProfileID                        id() const noexcept;
        const std::string                name() const noexcept;
        const std::optional<std::string> email() const noexcept;
    };

}   // namespace app

#endif   // __APP__DOMAIN__PROFILE_HPP__