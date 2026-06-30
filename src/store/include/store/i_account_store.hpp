#ifndef __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__
#define __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__

#include "config/id_types.hpp"
#include "config/strong_id.hpp"
#include "exceptions/base.hpp"
#include "finance/account/account.hpp"
#include "finance/account/accounts.hpp"
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
         * @brief Get a mapping of account IDs to their remapped IDs
         *
         * @return const IdIdMap<AccountId>&
         */
        [[nodiscard]]
        virtual const IdIdMap<AccountId>& getIdRemap() const = 0;

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
         * @brief Get all cash accounts
         *
         * @return std::vector<finance::Account>
         */
        [[nodiscard]] virtual std::vector<finance::Account> getCashAccounts(
        ) const = 0;

        /**
         * @brief Get all security accounts
         *
         * @return std::vector<finance::Account>
         */
        [[nodiscard]] virtual std::vector<finance::Account> getSecurityAccounts(
        ) const = 0;

        /**
         * @brief Get a mapping of account IDs to their names
         *
         * @return unorderedIdMap<AccountId, std::string>
         */
        [[nodiscard]]
        virtual unorderedIdMap<AccountId, std::string> getAccountIdToNameMap(
        ) const = 0;

        /**
         * @brief Get an external account by its currency
         *
         * @param currency
         * @return std::optional<AccountId>
         */
        [[nodiscard]]
        virtual std::optional<AccountId> getExternalAccount(
            Currency currency
        ) const = 0;

        /**
         * @brief Get all external account IDs
         *
         * @return IdSet<AccountId>
         */
        [[nodiscard]]
        virtual IdSet<AccountId> getExternalAccountIds() const = 0;

        /**
         * @brief Update the active profile
         *
         * @param profileIdOpt
         */
        virtual void updateActiveProfile(
            const std::optional<ProfileId>& profileIdOpt
        ) = 0;

        /**
         * @brief Get the current account session
         *
         * @return const finance::Accounts&
         */
        [[nodiscard]]
        virtual const finance::Accounts& getAccountSession() const = 0;
    };

}   // namespace store

#endif   // __STORE__INCLUDE__STORE__I_ACCOUNT_STORE_HPP__