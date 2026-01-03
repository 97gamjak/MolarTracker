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

        std::optional<Profile> getProfile(ProfileID id) const override;
        std::vector<Profile>   getAllProfiles() const override;

        ProfileID createProfile(const std::string& name) override;
        void renameProfile(ProfileID id, const std::string& newName) override;
        void deleteProfile(ProfileID id) override;

        ProfileID ensureDefaultProfile(const std::string& defaultName) override;
    };

}   // namespace app

#endif   // __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__