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
        std::optional<AccountId> id = AccountId::invalid();

        /// The name of the account (required)
        std::string name;

        /// The kind of the account (required)
        AccountKind kind;

        /// The currency of the account (required)
        Currency currency;
    };
}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__ACCOUNT_DRAFT_HPP__