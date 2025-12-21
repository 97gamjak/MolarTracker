#ifndef __APP__SERVICE_CONTAINER_HPP__
#define __APP__SERVICE_CONTAINER_HPP__

#include <memory>

#include "profile_service.hpp"

class RepoContainer;

class ServiceContainer
{
   private:
    ProfileService _profileService;

   public:
    explicit ServiceContainer(RepoContainer& repos);

    IProfileService&       profileService() noexcept;
    const IProfileService& profileService() const noexcept;
};

#endif   // __APP__SERVICE_CONTAINER_HPP__