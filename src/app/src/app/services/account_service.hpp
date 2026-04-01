#ifndef __APP__SRC__APP__SERVICES__ACCOUNT_SERVICE_HPP__
#define __APP__SRC__APP__SERVICES__ACCOUNT_SERVICE_HPP__

#include <memory>
#include <vector>

#include "app/services_api/i_account_service.hpp"

namespace app
{

    class IAccountRepo;   // forward declaration

    /**
     * @brief Implementation of Account service
     *
     */
    class AccountService : public IAccountService
    {
       private:
        /// reference to the account repository
        std::shared_ptr<IAccountRepo> _accountRepo;

       public:
        explicit AccountService(
            const std::shared_ptr<IAccountRepo>& accountRepo
        );

        [[nodiscard]] std::vector<finance::Account> getAllAccounts(
            const ProfileId& profileId
        ) const override;

        [[nodiscard]] std::vector<finance::Account> getAllCashAccounts(
            const ProfileId& profileId
        ) const override;

        [[nodiscard]] AccountId createCashAccount(
            const finance::Account& cashAccount,
            const ProfileId&        profileId
        ) override;
    };

}   // namespace app

#endif   // __APP__SRC__APP__SERVICES__ACCOUNT_SERVICE_HPP__