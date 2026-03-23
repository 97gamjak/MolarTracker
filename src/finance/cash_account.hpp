#ifndef __FINANCE__CASH_ACCOUNT_HPP__
#define __FINANCE__CASH_ACCOUNT_HPP__

#include <cstdint>
#include <string>
#include <variant>

#include "config/id_types.hpp"

enum class Currency : std::uint8_t;        // Forward declaration
enum class AccountStatus : std::uint8_t;   // Forward declaration

namespace finance
{

    /**
     * @brief A base class representing a financial account, which can be
     * extended to represent specific types of accounts (e.g., cash accounts,
     * security accounts, etc.). This class contains common attributes and
     * behaviors that are shared among all types of accounts.
     *
     */
    class Account
    {
       private:
        /// The unique identifier for the account
        AccountId _id;

        /// The status of the account (e.g., Active, Closed, etc.)
        AccountStatus _status;

        /// The current name of the account
        std::string _name;

        /// The current currency of the account
        Currency _currency;

       public:
        Account(
            AccountId     id,
            AccountStatus status,
            std::string   name,
            Currency      currency
        );

        void setId(AccountId id);

        [[nodiscard]] AccountId     getId() const;
        [[nodiscard]] AccountStatus getStatus() const;
        [[nodiscard]] std::string   getName() const;
        [[nodiscard]] Currency      getCurrency() const;
    };

    /**
     * @brief A class representing a cash account, which holds a certain amount
     * of cash in a specific currency. This class provides basic operations for
     * managing the cash balance, such as deposits and withdrawals.
     *
     */
    class CashAccount final : public Account
    {
       public:
        /**
         * @brief Inherit constructors from Account
         *
         * @param id the unique identifier for the cash account
         * @param status the status of the account (e.g., Active, Closed)
         * @param name the current name of the account
         * @param currency the current currency of the account
         */
        using Account::Account;
    };

    /// A variant type that can hold any type of account, this allows for
    /// managing different types of accounts in a single collection or
    /// interface, and the specific type of each account can be determined using
    /// std::visit or std::get_if on the variants.
    using AccountVariant = std::variant<CashAccount>;

}   // namespace finance

#endif   // __FINANCE__CASH_ACCOUNT_HPP__