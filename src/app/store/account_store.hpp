#ifndef __APP__STORE__ACCOUNT_STORE_HPP__
#define __APP__STORE__ACCOUNT_STORE_HPP__

#include <memory>
#include <set>
#include <vector>

#include "base/base_store.hpp"
#include "finance/cash_account.hpp"

namespace app
{

    class IAccountService;   // forward declaration

    /**
     * @brief Store for managing accounts
     *
     */
    class AccountStore : public BaseStore<finance::CashAccount, AccountId>
    {
       private:
        /// reference to the account service
        std::shared_ptr<IAccountService> _accountService;

        /// in-memory cache of cash accounts
        std::vector<finance::CashAccount> _cashAccounts;

        /// set of used account IDs to ensure uniqueness when generating new IDs
        std::set<AccountId> _usedIds;

        /// map of account IDs to their current state in the store (clean, new,
        std::unordered_map<AccountId, StoreState, AccountId::Hash>
            _accountStates;

       public:
        explicit AccountStore(
            const std::shared_ptr<IAccountService>& accountService
        );

        void reload();

        void commit() override;
    };

}   // namespace app

#endif   // __APP__STORE__ACCOUNT_STORE_HPP__