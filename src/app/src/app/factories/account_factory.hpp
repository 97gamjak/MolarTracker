#ifndef __APP__SRC__APP__FACTORIES__ACCOUNT_FACTORY_HPP__
#define __APP__SRC__APP__FACTORIES__ACCOUNT_FACTORY_HPP__

#include <vector>

#include "sql_models/account_row.hpp"

namespace finance
{
    class CashAccount;   // Forward declaration
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
        static finance::CashAccount toCashAccountDomain(
            const AccountRow& accountRow
        );

        static std::vector<finance::CashAccount> toCashAccountDomains(
            const std::vector<AccountRow>& accountRows
        );

        static std::pair<AccountRow, CashAccountRow> toCashAccountRow(
            const finance::CashAccount& account,
            const ProfileId&            profileId
        );
    };

}   // namespace app

#endif   // __APP__SRC__APP__FACTORIES__ACCOUNT_FACTORY_HPP__