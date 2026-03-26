#include "app/service_container.hpp"

#include "app/repo_container.hpp"
#include "services/account_service.hpp"
#include "services/profile_service.hpp"

namespace app
{

    /**
     * @brief Construct a new Service Container object
     *
     * @param repos
     */
    ServiceContainer::ServiceContainer(RepoContainer& repos)
        : _profileService{std::make_shared<ProfileService>(repos.getProfileRepo(
          ))},
          _accountService{std::make_shared<AccountService>(repos.getAccountRepo(
          ))}
    {
    }

    /**
     * @brief Get the Profile Service
     *
     * @return std::shared_ptr<IProfileService>
     */
    std::shared_ptr<IProfileService> ServiceContainer::getProfileService()
    {
        return _profileService;
    }

    /**
     * @brief Get the Profile Service (const version)
     *
     * @return std::shared_ptr<const IProfileService>
     */
    std::shared_ptr<const IProfileService> ServiceContainer::getProfileService(
    ) const
    {
        return _profileService;
    }

    /**
     * @brief Get the Account Service
     *
     * @return std::shared_ptr<IAccountService>
     */
    std::shared_ptr<IAccountService> ServiceContainer::getAccountService()
    {
        return _accountService;
    }

    /**
     * @brief Get the Account Service (const version)
     *
     * @return std::shared_ptr<const IAccountService>
     */
    std::shared_ptr<const IAccountService> ServiceContainer::getAccountService(
    ) const
    {
        return _accountService;
    }

}   // namespace app