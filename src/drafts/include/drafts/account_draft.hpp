#ifndef __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__

#include <optional>
#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"

namespace drafts
{
    /**
     * @brief A draft representation of an account
     *
     */
    struct AccountDraft
    {
        /// The ID of the account, this is optional because it may not be set
        /// when creating a new account
        AccountId id = AccountId::invalid();

        /// The name of the account (required)
        std::string name;

        /// The kind of the account (required)
        AccountKind kind;

        /// The currency of the account (required)
        Currency currency;

        /// The status of the account, this is optional because it may not be
        /// set
        std::optional<AccountStatus> status = std::nullopt;

        explicit AccountDraft(
            std::string _name,
            AccountKind _kind,
            Currency    _currency
        );

        explicit AccountDraft(
            AccountId                    _id,
            std::string                  _name,
            AccountKind                  _kind,
            Currency                     _currency,
            std::optional<AccountStatus> _status
        );
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__