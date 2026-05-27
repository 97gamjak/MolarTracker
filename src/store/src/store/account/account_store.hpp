#ifndef __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__
#define __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__

#include <memory>
#include <vector>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "domain/account.hpp"
#include "store/account/account_session.hpp"
#include "store/base/base_store.hpp"
#include "store/i_account_store.hpp"

namespace drafts
{
    struct AccountDraft;   // Forward declaration

}   // namespace drafts

namespace service
{
    class IAccountService;   // Forward declaration
}   // namespace service

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace store
{
    /**
     * @brief Store for managing accounts
     *
     */
    class AccountStore : public BaseStore<domain::Account, AccountId>,
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
        AccountSession _session;

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
        unorderedIdMap<AccountId, std::string> getAccountIdToNameMap(
        ) const override;

        [[nodiscard]]
        std::optional<AccountId> getExternalAccount(
            Currency currency
        ) const override;

        [[nodiscard]]
        idSet<AccountId> getExternalAccountIds() const override;

        void commit() override;

        void updateActiveProfile(
            const std::optional<ProfileId>& profileIdOpt
        ) override;

        [[nodiscard]]
        const AccountSession& getAccountSession() const override;

        [[nodiscard]]
        const unorderedIdMap<AccountId, AccountId>& getIdRemap() const override;

       private:
        void _refresh();
    };

}   // namespace store

#endif   // __STORE__SRC__STORE__ACCOUNT__ACCOUNT_STORE_HPP__