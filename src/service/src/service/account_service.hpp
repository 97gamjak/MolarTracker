#ifndef __SERVICE__SRC__SERVICE__ACCOUNT_SERVICE_HPP__
#define __SERVICE__SRC__SERVICE__ACCOUNT_SERVICE_HPP__

#include <memory>
#include <vector>

#include "service/i_account_service.hpp"

namespace repo
{
    class IAccountRepo;   // forward declaration
}   // namespace repo

namespace service
{
    /**
     * @brief Implementation of Account service
     *
     */
    class AccountService : public IAccountService
    {
       private:
        /// reference to the account repository
        std::shared_ptr<repo::IAccountRepo> _accountRepo;

       public:
        explicit AccountService(
            const std::shared_ptr<repo::IAccountRepo>& accountRepo
        );

        [[nodiscard]] std::vector<finance::Account> getAllAccounts(
            const ProfileId& profileId
        ) const override;

        [[nodiscard]] AccountId createAccount(
            const finance::Account& account,
            const ProfileId&        profileId
        ) override;
    };

}   // namespace service

#endif   // __SERVICE__SRC__SERVICE__ACCOUNT_SERVICE_HPP__