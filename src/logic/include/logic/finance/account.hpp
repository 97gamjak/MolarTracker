#ifndef __LOGIC__INCLUDE__LOGIC__FINANCE__ACCOUNT_HPP__
#define __LOGIC__INCLUDE__LOGIC__FINANCE__ACCOUNT_HPP__

#include <cstdint>
#include <string>

#include "config/finance.hpp"
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

        /// The kind of the account (e.g., Cash, External, Security)
        AccountKind _kind;

       public:
        Account(
            AccountId     id,
            AccountStatus status,
            std::string   name,
            Currency      currency,
            AccountKind   kind
        );

        void setId(AccountId id);

        [[nodiscard]] AccountId     getId() const;
        [[nodiscard]] AccountStatus getStatus() const;
        [[nodiscard]] std::string   getName() const;
        [[nodiscard]] Currency      getCurrency() const;
        [[nodiscard]] AccountKind   getKind() const;
        [[nodiscard]] bool          isExternal() const;
    };

}   // namespace finance

#endif   // __LOGIC__INCLUDE__LOGIC__FINANCE__ACCOUNT_HPP__