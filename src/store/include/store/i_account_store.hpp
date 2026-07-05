#ifndef __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__

#include "config/id_types.hpp"
#include "connections/connection.hpp"
#include "exceptions/base.hpp"
#include "finance/account/account.hpp"
#include "finance/account/accounts.hpp"
#include "subscriptions.hpp"
#include "utils/container/id_id_map.hpp"

namespace store
{
    enum class AccountStoreResult : std::uint8_t
    {
        Ok,
        Error,
        AccountNotFound,
        AccountNameConflict,
    };

    class AccountStoreException : public MolarTrackerException
    {
       public:
        using MolarTrackerException::MolarTrackerException;

        // TODO (97gamjak)[MOLTRACK-202]: implement this exception class and use
        // it in the store methods
    };

    /**
     * @brief Interface for reading accounts from the store
     *
     */
    class IAccountStoreReader
    {
       protected:
        /**
         * @brief Structure representing the commit event for account changes.
         *
         */
        struct OnCommit
        {
            /// Type alias for the commit callback function, which takes a
            /// reference to an IdIdMap of AccountId as its parameter.
            using func = std::function<void(const IdIdMap<AccountId>&)>;
        };

       public:
        virtual ~IAccountStoreReader() = default;

        /**
         * @brief Get an account by its ID
         *
         * @param id
         * @return std::optional<finance::Account>
         */
        [[nodiscard]]
        virtual std::optional<finance::Account> getAccount(
            AccountId id
        ) const = 0;

        /**
         * @brief Get all accounts
         *
         * @return std::vector<finance::Account>
         */
        [[nodiscard]] virtual std::vector<finance::Account> getAllAccounts(
        ) const = 0;

        [[nodiscard]]
        virtual Connection subscribeToCommit(
            OnCommit::func func,
            void*          subscriber
        ) = 0;

        SUBSCRIBE_VIRTUAL(finance::Account, AccountId)
    };

    /**
     * @brief Store for managing accounts
     *
     */
    class IAccountStore
    {
       public:
        virtual ~IAccountStore() = default;

        /**
         * @brief Create a new account
         *
         * @param account
         * @return AccountStoreResult
         */
        [[nodiscard]]
        virtual AccountStoreResult createAccount(
            const finance::Account& account
        ) = 0;

        /**
         * @brief Update the active profile
         *
         * @param profileIdOpt
         */
        virtual void updateActiveProfile(
            const std::optional<ProfileId>& profileIdOpt
        ) = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__