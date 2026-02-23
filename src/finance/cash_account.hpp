#ifndef __FINANCE__CASH_ACCOUNT_HPP__
#define __FINANCE__CASH_ACCOUNT_HPP__

#include <cstdint>

#include "config/id_types.hpp"
#include "currency.hpp"
#include "mstd/enum.hpp"

namespace finance
{

    // clang-format off
    // NOLINTBEGIN
    #define ACCOUNT_STATUS_LIST(X) \
        X(Active)                  \
        X(Closed)

    MSTD_ENUM(AccountStatus, uint8_t, ACCOUNT_STATUS_LIST);

    #define ACCOUNT_TYPE_LIST(X) \
        X(Cash)                  \
        X(Securities)            \
        X(External)

    MSTD_ENUM(AccountType, uint8_t, ACCOUNT_TYPE_LIST);
    // NOLINTEND
    // clang-format on

    /**
     * @brief A class representing a cash account, which holds a certain amount
     * of cash in a specific currency. This class provides basic operations for
     * managing the cash balance, such as deposits and withdrawals.
     *
     */
    class CashAccount
    {
       private:
        /// The unique identifier for the cash account
        AccountId _id;

        /// The type of the account (e.g., Cash, Securities, External)
        AccountType _type;

        /// The status of the account (e.g., Active, Closed)
        AccountStatus _status;

        /// The profile ID associated with the account
        ProfileId _profileId;

        /// The current currency of the account
        Currency _currency;

        /// The current name of the account
        std::string _name;

       public:
        explicit CashAccount(
            AccountId     id,
            AccountType   type,
            AccountStatus status,
            ProfileId     profileId,
            Currency      currency,
            std::string   name
        );
    };

}   // namespace finance

#endif   // __FINANCE__CASH_ACCOUNT_HPP__