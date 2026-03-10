#ifndef __APP__SERVICES__ACCOUNT_SERVICE_HPP__
#define __APP__SERVICES__ACCOUNT_SERVICE_HPP__

#include <memory>

#include "services_api/i_account_service.hpp"

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

        [[nodiscard]] AccountId createCashAccount(
            const finance::CashAccount& cashAccount
        ) override;
    };

}   // namespace app

#endif   // __APP__SERVICES__ACCOUNT_SERVICE_HPP__