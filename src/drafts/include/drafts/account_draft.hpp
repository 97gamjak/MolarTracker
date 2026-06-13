#ifndef __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__

#include <optional>
#include <string>

#include "config/id_types.hpp"
#include "utils/finance.hpp"

namespace drafts
{
    /**
     * @brief A draft representation of an account
     *
     */
    class AccountDraft
    {
       private:
        /// The ID of the account, this is optional because it may not be set
        /// when creating a new account
        AccountId _id = AccountId::invalid();

        /// The status of the account, this is optional because it may not be
        /// set
        std::optional<AccountStatus> _status = std::nullopt;

        /// The name of the account (required)
        std::string _name;

        /// The currency of the account (required)
        Currency _currency;

        /// The kind of the account (required)
        AccountKind _kind;

       public:
        explicit AccountDraft(
            AccountId                    id,
            std::optional<AccountStatus> status,
            std::string                  name,
            Currency                     currency,
            AccountKind                  kind
        );

        [[nodiscard]] AccountId                    getId() const;
        [[nodiscard]] std::optional<AccountStatus> getStatus() const;
        [[nodiscard]] const std::string&           getName() const;
        [[nodiscard]] AccountKind                  getKind() const;
        [[nodiscard]] Currency                     getCurrency() const;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__