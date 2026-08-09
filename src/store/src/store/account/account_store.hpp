#ifndef __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__
#define __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__

#include <memory>
#include <vector>

#include "config/id_types.hpp"
#include "finance/account/account.hpp"
#include "finance/account/accounts.hpp"
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
                         public IAccountStore
    {
       private:
        /// reference to the account service
        std::shared_ptr<service::IAccountService> _accountService;

        /// The ID of the active profile, this is used to determine which
        /// accounts to load and manage in the store
        ProfileId _activeProfileId = ProfileId::invalid();

        /// The session object for managing the session state of accounts in the
        /// store
        finance::Accounts _session;

        /// Connections for handling signals related to account store updates
        Connections _connections;

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
        [[nodiscard]]
        std::vector<finance::Account> getCashAccounts() const override;
        [[nodiscard]]
        std::vector<finance::Account> getSecurityAccounts() const override;
        [[nodiscard]]
        IdMap<AccountId, std::string> getAccountIdToNameMap() const override;

        [[nodiscard]]
        std::optional<AccountId> getExternalAccount(
            Currency currency
        ) const override;

        [[nodiscard]]
        IdSet<AccountId> getExternalAccountIds() const override;

        void commit();

        void updateActiveProfile(
            const std::optional<ProfileId>& profileIdOpt
        ) override;

        [[nodiscard]]
        const finance::Accounts& getAccountSession() const override;

        [[nodiscard]]
        const IdIdMap<AccountId>& getIdRemap() const override;

        void reload() override;

        [[nodiscard]]
        Connection subscribeToAccountStoreCommit(
            OnStoreCommit::func func,
            void*               user
        ) override;

        [[nodiscard]]
        FinanceResult<void> linkAccounts(AccountId id1, AccountId id2) override;

       private:
        void _refresh();
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__
