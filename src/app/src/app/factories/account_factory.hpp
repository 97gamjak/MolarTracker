#ifndef __APP__SRC__APP__FACTORIES__ACCOUNT_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__ACCOUNT_FACTORY_HPP__

#include <vector>

#include "sql_models/account_row.hpp"

namespace finance
{
    class Account;   // Forward declaration
}   // namespace finance

namespace app
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
        static finance::Account toAccountDomain(const AccountRow& accountRow);

        static std::vector<finance::Account> toAccountDomains(
            const std::vector<AccountRow>& accountRows
        );

        static std::pair<AccountRow, AccountDetailRow> toAccountRow(
            const finance::Account& account,
            const ProfileId&        profileId
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__ACCOUNT_FACTORY_HPP__