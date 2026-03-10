#ifndef __APP__FACTORIES__ACCOUNT_FACTORY_HPP__
#define __APP__FACTORIES__ACCOUNT_FACTORY_HPP__

#include "sql_models/account_row.hpp"

namespace finance
{
    class CashAccount;   // Forward declaration
}   // namespace finance

namespace app
{

    /**
     * @brief Factory class for converting between CashAccount domain models and
     * CashAccountRow database models, including the associated AccountRow for
     * the base account information.
     *
     */
    class CashAccountFactory
    {
       public:
        static std::pair<AccountRow, CashAccountRow> toRow(
            const finance::CashAccount& account
        );
    };

}   // namespace app

#endif   // __APP__FACTORIES__ACCOUNT_FACTORY_HPP__