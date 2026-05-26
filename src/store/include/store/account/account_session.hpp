#ifndef __STORE__INCLUDE__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__
#define __STORE__INCLUDE__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace domain
{
    class Account;   // Forward declaration
}   // namespace domain

namespace store
{
    /**
     * @brief Manages the session state of active accounts in the application.
     *
     */
    class AccountSession
    {
       private:
        /// A set of active account IDs currently in the session.
        idSet<AccountId> _activeAccounts;

       public:
        void set(const idSet<AccountId>& activeAccounts);
        void remove(const std::vector<AccountId>& accountIds);

        [[nodiscard]] const idSet<AccountId>& getIds() const;

        friend class AccountStore;

       private:
        void _add(const std::vector<domain::Account>& accounts);
    };
}   // namespace store

#endif   // __STORE__INCLUDE__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__