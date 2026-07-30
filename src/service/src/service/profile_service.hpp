#ifndef __SERVICE__SRC__SERVICE__PROFILE_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__PROFILE_SERVICE_HPP__

#include <memory>
#include <optional>
#include <vector>

#include "service/i_profile_service.hpp"

namespace repo
{
    class IProfileRepo;   // forward declaration
}   // namespace repo

namespace service
{

    /**
     * @brief Implementation of Profile service
     *
     */
    class ProfileService : public IProfileService
    {
       private:
        /// reference to the Profile repository
        std::shared_ptr<repo::IProfileRepo> _profileRepo;

       public:
        explicit ProfileService(
            const std::shared_ptr<repo::IProfileRepo>& repo
        );

        [[nodiscard]]
        std::optional<domain::Profile> get(ProfileId id) const override;

        [[nodiscard]]
        std::vector<domain::Profile> getAll() const override;

        [[nodiscard]]
        ProfileId create(const domain::Profile& profile) override;

        void update(
            ProfileId                         id,
            const std::string&                newName,
            const std::optional<std::string>& newEmail
        ) override;

        void remove(ProfileId id) override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__PROFILE_SERVICE_HPP__