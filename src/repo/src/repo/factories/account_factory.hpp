#ifndef __REPO__SRC__REPO__FACTORIES__ACCOUNT_FACTORY_HPP__
#define __REPO__SRC__REPO__FACTORIES__ACCOUNT_FACTORY_HPP__

#include <vector>

#include "sql_models/account_row.hpp"

namespace domain
{
    class Account;   // Forward declaration
}   // namespace domain

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
        [[nodiscard]] static domain::Account toAccountDomain(
            const AccountRow& accountRow
        );

        [[nodiscard]] static std::vector<domain::Account> toAccountDomains(
            const std::vector<AccountRow>& accountRows
        );

        [[nodiscard]] static AccountRow toAccountRow(
            const domain::Account& account,
            const ProfileId&       profileId
        );
    };

}   // namespace repo

#endif   // __REPO__SRC__REPO__FACTORIES__ACCOUNT_FACTORY_HPP__