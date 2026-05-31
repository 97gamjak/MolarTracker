#ifndef __STORE__SRC__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__
#define __STORE__SRC__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

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
        std::vector<finance::Account> _activeAccounts;

       public:
        void set(const std::vector<finance::Account>& activeAccounts);
        void add(const std::vector<finance::Account>& accounts);
        void remove(const std::vector<finance::Account>& accounts);
        void remove(const std::vector<AccountId>& ids);

        [[nodiscard]] bool contains(const finance::Account& account) const;

        [[nodiscard]] const std::vector<finance::Account>& getAccounts() const;
        [[nodiscard]] idSet<AccountId>                     getIds() const;
        [[nodiscard]] idSet<AccountId> getNonExternalIds() const;
    };
}   // namespace store

#endif   // __STORE__SRC__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__