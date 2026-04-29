#include "drafts/transaction_draft.hpp"

#include <utility>

#include "utils/timestamp.hpp"

namespace drafts
{

    /**
     * @brief Create a Cash Transaction Draft:: Create Cash Transaction Draft
     * object
     *
     * @param timestamp
     * @param entries
     * @param comment
     */
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

    /**
     * @brief Gets the entries of the cash transaction draft.
     *
     * @return const std::vector<TransactionEntryDraft>& The entries of the cash
     * transaction draft.
     */
    const std::vector<TransactionEntryDraft>& CreateCashTransactionDraft::
        getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Gets the timestamp of the cash transaction draft.
     *
     * @return const Timestamp& The timestamp of the cash transaction draft.
     */
    const Timestamp& CreateCashTransactionDraft::getTimestamp() const
    {
        return _timestamp;
    }

    /**
     * @brief Gets the comment of the cash transaction draft.
     *
     * @return const std::optional<std::string>& The comment of the cash
     * transaction draft.
     */
    const std::optional<std::string>& CreateCashTransactionDraft::getComment(
    ) const
    {
        return _comment;
    }

    /**
     * @brief Adds an entry to the cash transaction draft.
     *
     * @param entry The TransactionEntryDraft to add to the cash transaction
     * draft.
     */
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

    /**
     * @brief Set whether this transaction entry needs an external account, this
     * is used to indicate whether this transaction entry requires an additional
     * entry for an external account (e.g., for a withdrawal or deposit), which
     * will be automatically added when creating a cash transaction from the
     * draft. This allows the draft to specify that certain entries need to be
     * balanced with an external account, ensuring that the resulting
     * transaction is properly structured and balanced.
     *
     * @param needsExternal A boolean indicating whether this transaction entry
     * needs an external account, if true, an additional entry for an external
     * account will be added when creating a cash transaction from the draft.
     */
    void TransactionEntryDraft::setNeedsExternal(bool needsExternal)
    {
        _needsExternal = needsExternal;
    }

    /**
     * @brief get the account ID associated with this transaction entry draft
     *
     * @return AccountId
     */
    AccountId TransactionEntryDraft::getAccountId() const { return _accountId; }

    /**
     * @brief get the cash amount associated with this transaction entry draft
     *
     * @return finance::Cash
     */
    finance::Cash TransactionEntryDraft::getCash() const { return _cash; }

    /**
     * @brief get whether this transaction entry draft needs an external account
     *
     * @return bool
     */
    bool TransactionEntryDraft::needsExternal() const { return _needsExternal; }

    /**
     * @brief Construct a new Transaction Overview Draft:: Transaction Overview
     * Draft object
     *
     * @param timestamp
     * @param entries
     * @param comment
     */
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

    /**
     * @brief Gets the timestamp of the transaction overview draft.
     *
     * @return const Timestamp& The timestamp of the transaction overview draft.
     */
    const Timestamp& TransactionOverviewDraft::getTimestamp() const
    {
        return _timestamp;
    }

    /**
     * @brief Gets the entries of the transaction overview draft.
     *
     * @return const std::vector<TransactionEntryDraft>& The entries of the
     * transaction overview draft.
     */
    const std::vector<TransactionEntryDraft>& TransactionOverviewDraft::
        getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Gets the comment of the transaction overview draft.
     *
     * @return const std::optional<std::string>& The comment of the transaction
     * overview draft.
     */
    const std::optional<std::string>& TransactionOverviewDraft::getComment(
    ) const
    {
        return _comment;
    }

}   // namespace drafts