#ifndef __APP__FACTORIES__ACCOUNT_FACTORY_HPP__
#define __APP__FACTORIES__ACCOUNT_FACTORY_HPP__

#include "sql_models/account_row.hpp"

namespace finance
{
    class CashAccount;   // Forward declaration
}

namespace app
{

    /**
     * @brief Factory class for converting between Account domain models and
     * AccountRow database models, this class provides static methods for
     * converting between the CashAccount domain model and the corresponding
     * AccountRow and CashAccountRow database models, it serves as a centralized
     * place for handling all conversions related to accounts, ensuring that the
     * conversion logic is consistent and maintainable across the application.
     *
     */
    class AccountFactory
    {
       public:
        /// Convert a CashAccount domain model to an AccountRow database model,
        /// this method should take a CashAccount object and return an
        /// AccountRow object with the appropriate fields set based on the
        /// properties of the CashAccount, this may involve setting the kind
        /// field to indicate that it is a cash account, and any other necessary
        /// fields for the base account information.
        static std::pair<AccountRow, CashAccountRow> fromCashAccount(
            const finance::CashAccount& account
        );
    };

}   // namespace app