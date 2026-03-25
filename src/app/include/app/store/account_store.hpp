#ifndef __APP__STORE__ACCOUNT_STORE_HPP__
#define __APP__STORE__ACCOUNT_STORE_HPP__

#include <memory>

#include "base/base_store.hpp"
#include "exceptions/base.hpp"
#include "finance/cash_account.hpp"

namespace drafts
{
    struct AccountDraft;   // Forward declaration

}   // namespace drafts

namespace app
{

    class IAccountService;   // forward declaration

    enum class AccountStoreResult : std::uint8_t
    {
        Ok,
        Error,
        AccountNotFound,
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
    class AccountStore : public BaseStore<finance::AccountVariant, AccountId>
    {
       private:
        /// reference to the account service
        std::shared_ptr<IAccountService> _accountService;

        /// The ID of the active profile, this is used to determine which
        /// accounts to load and manage in the store
        ProfileId _activeProfileId = ProfileId::invalid();

       public:
        explicit AccountStore(
            const std::shared_ptr<IAccountService>& accountService
        );

        [[nodiscard]] AccountStoreResult createAccount(
            const drafts::AccountDraft& accountDraft
        );

        [[nodiscard]] std::vector<const finance::Account*> getAllAccounts(
        ) const;

        void commit() override;

        void updateActiveProfile(const std::optional<ProfileId>& profileIdOpt);

       private:
        [[nodiscard]] AccountStoreResult _addCashAccount(
            const drafts::AccountDraft& accountDraft
        );

        void _refresh();
    };

}   // namespace app

#endif   // __APP__STORE__ACCOUNT_STORE_HPP__