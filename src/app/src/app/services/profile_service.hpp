#ifndef __APP__SRC__APP__SERVICES__PROFILE_SERVICE_HPP__
#define __APP__SRC__APP__SERVICES__PROFILE_SERVICE_HPP__

#include <memory>
#include <optional>
#include <vector>

#include "app/services_api/i_profile_service.hpp"

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
        /// reference to the Profile repository
        std::shared_ptr<IProfileRepo> _profileRepo;

       public:
        explicit ProfileService(const std::shared_ptr<IProfileRepo>& repo);

        [[nodiscard]] std::optional<Profile> get(ProfileId id) const override;
        [[nodiscard]] std::vector<Profile>   getAll() const override;

        [[nodiscard]] ProfileId create(
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

#endif   // __APP__SRC__APP__SERVICES__PROFILE_SERVICE_HPP__