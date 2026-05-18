#ifndef __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_SESSION__HPP__
#define __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_SESSION__HPP__

#include <vector>

#include "config/id_types.hpp"

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace app
{
    class AccountSession
    {
       private:
        idSet<AccountId> _activeAccounts;

       public:
        void set(const idSet<AccountId>& activeAccounts);
        void add(const std::vector<finance::Account>& accounts);
        void remove(const std::vector<AccountId>& accountIds);

        [[nodiscard]] const idSet<AccountId>& getIds() const;
    };
}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__ACCOUNT__ACCOUNT_SESSION__HPP__