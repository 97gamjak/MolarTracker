#ifndef __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__
#define __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__

#include <optional>
#include <vector>

#include "services_api/i_profile_service.hpp"

namespace app
{

    class IProfileRepo;

    class ProfileService : public IProfileService
    {
       private:
        IProfileRepo& _profileRepo;

       public:
        explicit ProfileService(IProfileRepo& profileRepo);

        std::optional<Profile> getProfile(ProfileId id) const override;
        std::vector<Profile>   getAllProfiles() const override;

        ProfileId createProfile(
            const std::string&                name,
            const std::optional<std::string>& email
        ) override;
        void renameProfile(ProfileId id, const std::string& newName) override;
        void deleteProfile(ProfileId id) override;

        bool profileExists(const std::string& name) const;
    };

}   // namespace app

#endif   // __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__