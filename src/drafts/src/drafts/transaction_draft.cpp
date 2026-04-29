#include "drafts/transaction_draft.hpp"

#include <utility>

#include "utils/timestamp.hpp"

namespace drafts
{

    CreateCashTransactionDraft::CreateCashTransactionDraft(
        Timestamp                          timestamp,
        std::vector<TransactionEntryDraft> entries,
        std::optional<std::string>         comment
    )
        : _timestamp(timestamp),
          _entries(std::move(entries)),
          _comment(std::move(comment))
    {
    }

    const std::vector<TransactionEntryDraft>& CreateCashTransactionDraft::
        getEntries() const
    {
        return _entries;
    }

    const Timestamp& CreateCashTransactionDraft::getTimestamp() const
    {
        return _timestamp;
    }

    const std::optional<std::string>& CreateCashTransactionDraft::getComment(
    ) const
    {
        return _comment;
    }

    void CreateCashTransactionDraft::addEntry(
        const TransactionEntryDraft& entry
    )
    {
        _entries.push_back(entry);
    }

    /**
     * @brief Construct a new Transaction Entry Draft:: Transaction Entry Draft
     * object
     *
     * @param accountId_
     * @param cash_
     */
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId     accountId_,
        finance::Cash cash_
    )
        : accountId(accountId_), cash(cash_)
    {
    }
}   // namespace drafts