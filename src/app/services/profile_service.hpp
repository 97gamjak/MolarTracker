#ifndef __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__
#define __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__

#include <optional>
#include <vector>

#include "services_api/i_profile_service.hpp"

namespace app
{

    class IProfileRepo;   // forward declaration

    /**
     * @brief Implementation of Profile service
     *
     */
    class ProfileService : public IProfileService
    {
       private:
        IProfileRepo& _profileRepo;

       public:
        explicit ProfileService(IProfileRepo& getProfileRepo);

        std::optional<Profile> get(ProfileId id) const override;
        std::vector<Profile>   getAll() const override;

        ProfileId create(
            const std::string&                name,
            const std::optional<std::string>& email
        ) override;

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override;

        void remove(ProfileId id) override;
    };

}   // namespace app

#endif   // __APP__SERVICES__PROFILE_SERVICE_IMPL_HPP__