#ifndef __APP__INCLUDE__APP__SERVICE_CONTAINER_HPP__
#define __APP__INCLUDE__APP__SERVICE_CONTAINER_HPP__

#include <memory>

namespace app
{

    class RepoContainer;         // Forward declaration
    class IProfileService;       // Forward declaration
    class IAccountService;       // Forward declaration
    class ITransactionService;   // Forward declaration

    /**
     * @brief Container for all services
     *
     */
    class ServiceContainer
    {
       private:
        /// The Profile service
        std::shared_ptr<IProfileService> _profileService;
        /// The Account service
        std::shared_ptr<IAccountService> _accountService;
        /// The Transaction service
        std::shared_ptr<ITransactionService> _transactionService;

       public:
        explicit ServiceContainer(RepoContainer& repos);

        [[nodiscard]] std::shared_ptr<IProfileService> getProfileService();
        [[nodiscard]] std::shared_ptr<const IProfileService> getProfileService(
        ) const;

        [[nodiscard]] std::shared_ptr<IAccountService> getAccountService();
        [[nodiscard]] std::shared_ptr<const IAccountService> getAccountService(
        ) const;

        [[nodiscard]] std::shared_ptr<ITransactionService> getTransactionService(
        );
        [[nodiscard]] std::shared_ptr<const ITransactionService> getTransactionService(
        ) const;
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__SERVICE_CONTAINER_HPP__