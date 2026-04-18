#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__

#include <optional>
#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "utils/timestamp.hpp"

namespace drafts
{

    struct TransactionEntryDraft;   // Forward declaration

    /**
     * @brief A draft representation of a transaction
     *
     */
    struct TransactionDraft
    {
        /// The ID of the transaction
        TransactionId id = TransactionId::invalid();

        /// The timestamp of the transaction
        Timestamp timestamp;

        /// An optional comment for the transaction
        std::optional<std::string> comment;

        /// The entries of the transaction
        std::vector<TransactionEntryDraft> entries;
    };

    /**
     * @brief A draft representation of a transaction entry
     *
     */
    struct TransactionEntryDraft
    {
        /// The ID of the account associated with the transaction entry
        AccountId accountId;

        /// The cash amount associated with the transaction entry
        finance::Cash cash;

        /// The kind of account associated with the transaction entry
        AccountKind accountKind;

        /// A flag indicating whether this transaction entry needs an external
        /// account
        bool needsExternal = false;

        TransactionEntryDraft(
            AccountId     accountId_,
            finance::Cash cash_,
            AccountKind   accountKind_
        );
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__