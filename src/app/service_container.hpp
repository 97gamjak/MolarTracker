#ifndef __APP__SERVICE_CONTAINER_HPP__
#define __APP__SERVICE_CONTAINER_HPP__

#include <memory>

#include "services/profile_service.hpp"

namespace app
{

    class RepoContainer;   // Forward declaration

    /**
     * @brief Container for all services
     *
     */
    class ServiceContainer
    {
       private:
        ProfileService _profileService;

       public:
        explicit ServiceContainer(RepoContainer& repos);

        IProfileService&       getProfileService();
        const IProfileService& getProfileService() const;
    };

}   // namespace app

#endif   // __APP__SERVICE_CONTAINER_HPP__