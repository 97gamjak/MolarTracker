#ifndef __APP__STORE__ACCOUNT_STORE_HPP__
#define __APP__STORE__ACCOUNT_STORE_HPP__

#include <memory>

#include "i_store.hpp"

namespace app
{

    class IAccountService;   // forward declaration

    /**
     * @brief Store for managing accounts
     *
     */
    class AccountStore : public IStore
    {
       private:
        /// reference to the account service
        std::shared_ptr<IAccountService> _accountService;

       public:
        explicit AccountStore(
            const std::shared_ptr<IAccountService>& accountService
        );

        void reload();

        [[nodiscard]] bool isDirty() const override;
        void               commit() override;
    };

}   // namespace app

#endif   // __APP__STORE__ACCOUNT_STORE_HPP__