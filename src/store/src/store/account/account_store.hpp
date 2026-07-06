#ifndef __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__
#define __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__

#include <memory>
#include <vector>

#include "config/id_types.hpp"
#include "finance/account/account.hpp"
#include "store/base/base_store.hpp"
#include "store/i_account_store.hpp"

namespace service
{
    class IAccountService;   // forward declaration
}   // namespace service

namespace store
{
    /**
     * @brief Store for managing accounts
     *
     */
    class AccountStore : public BaseStore<finance::Account, AccountId>,
                         public IAccountStore,
                         public IAccountStoreReader
    {
       private:
        /// reference to the account service
        std::shared_ptr<service::IAccountService> _accountService;

        /// The ID of the active profile, this is used to determine which
        /// accounts to load and manage in the store
        ProfileId _activeProfileId = ProfileId::invalid();

        /// Connections for handling signals related to account store updates
        Connections _connections;

        /// Observable for commit events, this allows other parts of the
        /// application to subscribe to commit events and react accordingly when
        /// changes are committed in the account store
        Observable<OnCommit, OnProfileChanged> _observable;

       public:
        explicit AccountStore(
            const std::shared_ptr<service::IAccountService>& accountService
        );

        [[nodiscard]]
        AccountStoreResult createAccount(
            const finance::Account& account
        ) override;

        [[nodiscard]]
        std::optional<finance::Account> getAccount(AccountId id) const override;
        [[nodiscard]]
        std::vector<finance::Account> getAllAccounts() const override;

        void commit();

        void updateActiveProfile(
            const std::optional<ProfileId>& profileIdOpt
        ) override;

        [[nodiscard]]
        Connection subscribeToCommit(
            OnCommit::func func,
            void*          subscriber
        ) override;

        [[nodiscard]]
        Connection subscribeToProfileChanged(
            OnProfileChanged::func func,
            void*                  subscriber
        ) override;

        SUBSCRIBE_OVERRIDE(finance::Account, AccountId)

       private:
        void _refresh();
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__