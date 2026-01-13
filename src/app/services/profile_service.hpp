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

        std::optional<Profile> get(ProfileId id) const override;
        std::vector<Profile>   getAll() const override;

        ProfileId create(
            const std::string&                name,
            const std::optional<std::string>& email
        ) override;
        void rename(ProfileId id, const std::string& newName) override;
        void remove(ProfileId id) override;
    };

}   // namespace app

#endif   // __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__