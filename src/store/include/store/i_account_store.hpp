#ifndef __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__

#include "config/id_types.hpp"
#include "connections/connection.hpp"
#include "exceptions/base.hpp"
#include "finance/account/account.hpp"
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

        /**
         * @brief Structure representing the profile changed event for account
         * changes.
         *
         */
        struct OnProfileChanged
        {
            /// Type alias for the profile changed callback function, which
            /// takes no parameters and returns void.
            using func = std::function<void()>;
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

        /**
         * @brief Subscribe to commit events in the account store, this allows
         * other parts of the application to be notified when changes are
         * committed in the account store, enabling them to react accordingly,
         * such as updating UI elements or triggering other actions based on
         * the committed changes.
         *
         * @param func The callback function to be called when a commit event
         * occurs, this function should accept a reference to an IdIdMap of
         * AccountId, which represents the mapping of old account IDs to new
         * account IDs after the commit, allowing the subscriber to understand
         * how the account IDs have changed as a result of the commit.
         * @param subscriber A pointer to the subscriber object that is
         * subscribing to the commit event, this is used for managing the
         * lifetime of the subscription and ensuring that it is properly cleaned
         * up when the subscriber is destroyed.
         *
         * @return Connection A connection object representing the subscription,
         * which can be used to manage the subscription and disconnect it when
         * no longer needed.
         */
        [[nodiscard]]
        virtual Connection subscribeToCommit(
            OnCommit::func func,
            void*          subscriber
        ) = 0;

        /**
         * @brief Subscribe to profile changed events in the account store, this
         * allows other parts of the application to be notified when the active
         * profile changes in the account store, enabling them to react
         * accordingly, such as updating UI elements or triggering other actions
         * based on the new active profile.
         *
         * @param func The callback function to be called when a profile changed
         * event occurs, this function should accept no parameters and return
         * void, allowing the subscriber to perform any necessary actions in
         * response to the profile change.
         * @param subscriber A pointer to the subscriber object that is
         * subscribing to the profile changed event, this is used for managing
         * the lifetime of the subscription and ensuring that it is properly
         * cleaned up when the subscriber is destroyed.
         *
         * @return Connection A connection object representing the subscription,
         * which can be used to manage the subscription and disconnect it when
         * no longer needed.
         */
        [[nodiscard]]
        virtual Connection subscribeToProfileChanged(
            OnProfileChanged::func func,
            void*                  subscriber
        ) = 0;

        /// @cond DOXYGEN_IGNORE
        SUBSCRIBE_VIRTUAL(finance::Account, AccountId)
        /// @endcond
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