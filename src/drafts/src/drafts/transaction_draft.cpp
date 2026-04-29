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
     * @param accountId
     * @param cash
     */
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId     accountId,
        finance::Cash cash
    )
        : _accountId(accountId), _cash(cash)
    {
    }

    void TransactionEntryDraft::setNeedsExternal(bool needsExternal)
    {
        _needsExternal = needsExternal;
    }

    AccountId TransactionEntryDraft::getAccountId() const { return _accountId; }

    finance::Cash TransactionEntryDraft::getCash() const { return _cash; }

    bool TransactionEntryDraft::needsExternal() const { return _needsExternal; }

    TransactionOverviewDraft::TransactionOverviewDraft(
        Timestamp                          timestamp,
        std::vector<TransactionEntryDraft> entries,
        std::optional<std::string>         comment
    )
        : _timestamp(timestamp),
          _entries(std::move(entries)),
          _comment(std::move(comment))
    {
    }

    const Timestamp& TransactionOverviewDraft::getTimestamp() const
    {
        return _timestamp;
    }

    const std::vector<TransactionEntryDraft>& TransactionOverviewDraft::
        getEntries() const
    {
        return _entries;
    }

    const std::optional<std::string>& TransactionOverviewDraft::getComment(
    ) const
    {
        return _comment;
    }

}   // namespace drafts