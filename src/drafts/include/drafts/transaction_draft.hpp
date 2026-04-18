#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__

#include <optional>
#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
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
        TransactionId id = TransactionId::invalid();

        Timestamp timestamp;

        std::optional<std::string> comment;

        std::vector<TransactionEntryDraft> entries;
    };

    struct TransactionEntryDraft
    {
        AccountId accountId;

        micro_units amount;

        Currency currency;

        AccountKind accountKind;

        bool needsExternal = false;

        TransactionEntryDraft(
            AccountId   accountId,
            micro_units amount,
            Currency    currency,
            AccountKind accountKind
        );
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__