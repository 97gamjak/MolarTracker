#ifndef __APP__STORE__ACCOUNT_STORE_HPP__
#define __APP__STORE__ACCOUNT_STORE_HPP__

#include <memory>

#include "base/base_store.hpp"
#include "finance/cash_account.hpp"

namespace app
{

    class IAccountService;   // forward declaration

    /**
     * @brief Store for managing accounts
     *
     */
    class AccountStore : public BaseStore<finance::AccountVariant, AccountId>
    {
       private:
        /// reference to the account service
        std::shared_ptr<IAccountService> _accountService;

        /// The ID of the active profile, this is used to determine which
        /// accounts to load and manage in the store
        ProfileId _activeProfileId;

       public:
        explicit AccountStore(
            const std::shared_ptr<IAccountService>& accountService
        );

        void commit() override;
    };

}   // namespace app

#endif   // __APP__STORE__ACCOUNT_STORE_HPP__