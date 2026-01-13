#ifndef __APP__SERVICE_CONTAINER_HPP__
#define __APP__SERVICE_CONTAINER_HPP__

#include <memory>

#include "services/profile_service.hpp"

namespace app
{

    class RepoContainer;

    class ServiceContainer
    {
       private:
        ProfileService _profileService;

       public:
        explicit ServiceContainer(RepoContainer& repos);

        IProfileService&       profileService();
        const IProfileService& profileService() const;
    };

}   // namespace app

#endif   // __APP__SERVICE_CONTAINER_HPP__