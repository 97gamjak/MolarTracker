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

        virtual std::optional<Profile> getProfile(ProfileID id) const = 0;
        virtual std::vector<Profile>   getAllProfiles() const         = 0;

        virtual ProfileID createProfile(const std::string& name) = 0;
        virtual void      renameProfile(
                 ProfileID          id,
                 const std::string& newName
             )                                   = 0;
        virtual void deleteProfile(ProfileID id) = 0;

        virtual ProfileID ensureDefaultProfile(
            const std::string& defaultName
        ) = 0;
    };

}   // namespace app

#endif   // __APP__SERVICES__PROFILE_SERVICE_HPP__