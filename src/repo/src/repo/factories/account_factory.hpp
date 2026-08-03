#ifndef __REPO__SRC__REPO__FACTORIES__ACCOUNT_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__ACCOUNT_FACTORY_HPP__

#include <vector>

#include "sql_models/account_row.hpp"

namespace finance
{
    class Account;       // Forward declaration
    class CashAccount;   // Forward declaration
}   // namespace finance

namespace repo
{

    /**
     * @brief Factory class for converting between Account domain models and
     * AccountRow database models, including the associated CashAccountRow for
     * the specific account type information.
     *
     */
    class AccountFactory
    {
       public:
        [[nodiscard]]
        static finance::Account toAccountDomain(const AccountRow& accountRow);

        [[nodiscard]]
        static std::vector<finance::Account> toAccountDomains(
            const std::vector<AccountRow>& accountRows
        );

        [[nodiscard]]
        static AccountRow toAccountRow(
            const finance::Account& account,
            const ProfileId&        profileId
        );

        [[nodiscard]]
        static CashAccountDetailRow toCashAccountDetailRow(
            const finance::CashAccount& account,
            const AccountId&            accountId
        );
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__ACCOUNT_FACTORY_HPP__
