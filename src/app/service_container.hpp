#ifndef __APP__SERVICE_CONTAINER_HPP__
#define __APP__SERVICE_CONTAINER_HPP__

#include <memory>

namespace app
{

    class RepoContainer;     // Forward declaration
    class IProfileService;   // Forward declaration
    class IAccountService;   // Forward declaration

    /**
     * @brief Container for all services
     *
     */
    class ServiceContainer
    {
       private:
        /// The Profile service
        std::shared_ptr<IProfileService> _profileService;
        std::shared_ptr<IAccountService> _accountService;

       public:
        explicit ServiceContainer(RepoContainer& repos);

        [[nodiscard]] std::shared_ptr<IProfileService> getProfileService();
        [[nodiscard]] std::shared_ptr<const IProfileService> getProfileService(
        ) const;

        [[nodiscard]] std::shared_ptr<IAccountService> getAccountService();
        [[nodiscard]] std::shared_ptr<const IAccountService> getAccountService(
        ) const;
    };

}   // namespace app

#endif   // __APP__SERVICE_CONTAINER_HPP__