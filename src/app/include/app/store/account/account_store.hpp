#ifndef __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_STORE_HPP__
#define __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_STORE_HPP__

#include <memory>
#include <vector>

#include "app/store/account/account_session.hpp"
#include "app/store/base/base_store.hpp"
#include "config/id_types.hpp"
#include "exceptions/base.hpp"
#include "finance/account.hpp"

namespace drafts
{
    struct AccountDraft;   // Forward declaration

}   // namespace drafts

namespace service
{
    class IAccountService;   // forward declaration
}   // namespace service

namespace app
{
    enum class AccountStoreResult : std::uint8_t
    {
        Ok,
        Error,
        AccountNotFound,
        AccountNameConflict,
    };

    class AccountStoreException : MolarTrackerException
    {
       public:
        using MolarTrackerException::MolarTrackerException;

        // TODO (97gamjak)[MOLTRACK-202]: implement this exception class and use
        // it in the store methods
    };

    /**
     * @brief Store for managing accounts
     *
     */
    class AccountStore : public BaseStore<finance::Account, AccountId>
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
        AccountStoreResult createAccount(const drafts::AccountDraft& account);

        [[nodiscard]]
        std::optional<drafts::AccountDraft> getAccount(AccountId id) const;
        [[nodiscard]]
        std::vector<drafts::AccountDraft> getAllAccounts() const;
        [[nodiscard]]
        std::vector<drafts::AccountDraft> getCashAccounts() const;
        [[nodiscard]]
        std::vector<drafts::AccountDraft> getSecurityAccounts() const;
        [[nodiscard]]
        std::unordered_map<
            AccountId,
            std::string,
            AccountId::Hash> getAccountIdToNameMap() const;

        [[nodiscard]]
        std::optional<AccountId> getExternalAccount(Currency currency) const;

        [[nodiscard]]
        idSet<AccountId> getExternalAccountIds() const;

        void commit();

        void updateActiveProfile(const std::optional<ProfileId>& profileIdOpt);

        [[nodiscard]] const AccountSession& getAccountSession() const;

       private:
        void _refresh();
    };

}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_STORE_HPP__