#ifndef __APP__SERVICES__PROFILE_SERVICE_HPP__
#define __APP__SERVICES__PROFILE_SERVICE_HPP__

#include <optional>
#include <string>
#include <vector>

#include "config/id_types.hpp"

namespace app
{

    class Profile;

    class IProfileService
    {
       public:
        virtual ~IProfileService() = default;

        virtual std::optional<Profile> get(ProfileId id) const = 0;
        virtual std::vector<Profile>   getAll() const          = 0;

        virtual ProfileId create(
            const std::string&                name,
            const std::optional<std::string>& email
        )                                                     = 0;
        virtual void rename(ProfileId id, const std::string&) = 0;
        virtual void remove(ProfileId id)                     = 0;
    };

}   // namespace app

#endif   // __APP__SERVICES__PROFILE_SERVICE_HPP__