#ifndef __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__
#define __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace app
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
        void add(const std::vector<finance::Account>& accounts);
        void remove(const std::vector<AccountId>& accountIds);

        [[nodiscard]] const idSet<AccountId>& getIds() const;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_SESSION_HPP__