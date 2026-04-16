#ifndef __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__
#define __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__

#include <optional>
#include <string>

#include "utils/timestamp.hpp"

namespace drafts
{

    /**
     * @brief A draft representation of a transaction
     *
     */
    struct TransactionDraft
    {
        Timestamp timestamp;

        std::optional<std::string> comment;
    };

}   // namespace drafts

#endif   // __DRAFTS__INCLUDE__DRAFTS__TRANSACTION_DRAFT_HPP__