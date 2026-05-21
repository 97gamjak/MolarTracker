#include "drafts/transaction_draft.hpp"

#include <utility>

#include "config/id_types.hpp"
#include "utils/timestamp.hpp"

namespace drafts
{

    /**
     * @brief Construct a new Trade Leg Draft:: Trade Leg Draft object
     *
     * @param accountId
     * @param unitPrice
     * @param quantity
     * @param ticker
     */
    TradeLegDraft::TradeLegDraft(
        AccountId     accountId,
        finance::Cash unitPrice,
        Quantity      quantity,
        std::string   ticker
    )
        : _accountId(accountId),
          _unitPrice(unitPrice),
          _quantity(quantity),
          _ticker(std::move(ticker))
    {
    }

    /**
     * @brief Get the account ID associated with the trade leg draft.
     *
     * @return AccountId The account ID associated with the trade leg draft.
     */
    AccountId TradeLegDraft::getAccountId() const { return _accountId; }

    /**
     * @brief Get the unit price associated with the trade leg draft.
     *
     * @return finance::Cash The unit price associated with the trade leg draft.
     */
    finance::Cash TradeLegDraft::getUnitPrice() const { return _unitPrice; }

    /**
     * @brief Get the quantity associated with the trade leg draft.
     *
     * @return Quantity The quantity associated with the trade leg draft.
     */
    Quantity TradeLegDraft::getQuantity() const { return _quantity; }

    /**
     * @brief Get the ticker associated with the trade leg draft.
     *
     * @return const std::string& The ticker associated with the trade leg
     * draft.
     */
    const std::string& TradeLegDraft::getTicker() const { return _ticker; }

    /**
     * @brief Get the instrument ID associated with the trade leg draft.
     *
     * @return InstrumentId The instrument ID associated with the trade leg
     * draft.
     */
    InstrumentId TradeLegDraft::getInstrumentId() const
    {
        return _instrumentId;
    }

    /**
     * @brief set the position ID associated with the trade leg draft
     *
     * @param positionId
     */
    void TradeLegDraft::setPositionId(PositionId positionId)
    {
        _positionId = positionId;
    }

    /**
     * @brief Get the position ID associated with the trade leg draft.
     *
     * @return PositionId The position ID associated with the trade leg draft.
     */
    PositionId TradeLegDraft::getPositionId() const { return _positionId; }

    /**
     * @brief Get the currency associated with the trade leg draft.
     *
     * @return Currency The currency associated with the trade leg draft.
     */
    Currency TradeLegDraft::getCurrency() const
    {
        return _unitPrice.getCurrency();
    }

    /**
     * @brief Set the instrument ID associated with the trade leg draft.
     *
     * @param instrumentId The instrument ID to associate with the trade leg
     * draft.
     */
    void TradeLegDraft::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    /**
     * @brief Create a Cash Transaction Draft:: Create Cash Transaction Draft
     * object
     *
     * @param timestamp
     * @param entries
     * @param comment
     */
    CreateTransactionDraft::CreateTransactionDraft(
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
    const std::vector<TransactionEntryDraft>& CreateTransactionDraft::
        getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Gets the timestamp of the cash transaction draft.
     *
     * @return const Timestamp& The timestamp of the cash transaction draft.
     */
    const Timestamp& CreateTransactionDraft::getTimestamp() const
    {
        return _timestamp;
    }

    /**
     * @brief Gets the comment of the cash transaction draft.
     *
     * @return const std::optional<std::string>& The comment of the cash
     * transaction draft.
     */
    const std::optional<std::string>& CreateTransactionDraft::getComment() const
    {
        return _comment;
    }

    /**
     * @brief Adds an entry to the cash transaction draft.
     *
     * @param entry The TransactionEntryDraft to add to the cash transaction
     * draft.
     */
    void CreateTransactionDraft::addEntry(const TransactionEntryDraft& entry)
    {
        _entries.push_back(entry);
    }

    /**
     * @brief Adds multiple entries to the cash transaction draft.
     *
     * @param entries The vector of TransactionEntryDraft to add to the cash
     * transaction draft.
     */
    void CreateTransactionDraft::addEntries(
        const std::vector<TransactionEntryDraft>& entries
    )
    {
        _entries.insert(_entries.end(), entries.begin(), entries.end());
    }

    /**
     * @brief Create a Stock Transaction Draft:: Create Stock Transaction Draft
     * object
     *
     * @param timestamp
     * @param entries
     * @param legs
     * @param comment
     */
    CreateStockTransactionDraft::CreateStockTransactionDraft(
        Timestamp                          timestamp,
        std::vector<TransactionEntryDraft> entries,
        std::vector<TradeLegDraft>         legs,
        std::optional<std::string>         comment
    )
        : CreateTransactionDraft(
              timestamp,
              std::move(entries),
              std::move(comment)
          ),
          _legs(std::move(legs))
    {
    }

    /**
     * @brief Get the legs associated with the stock transaction draft.
     *
     * @return const std::vector<TradeLegDraft>& The legs associated with the
     * stock transaction draft.
     */
    const std::vector<TradeLegDraft>& CreateStockTransactionDraft::getLegs(
    ) const
    {
        return _legs;
    }

    /**
     * @brief Get the legs associated with the stock transaction draft.
     *
     * @return std::vector<TradeLegDraft>& The legs associated with the stock
     * transaction draft.
     */
    std::vector<TradeLegDraft>& CreateStockTransactionDraft::getLegs()
    {
        return _legs;
    }

    /**
     * @brief Construct a new Transaction Entry Draft:: Transaction Entry Draft
     * object
     *
     * @param accountId
     * @param cash
     * @param type
     * @param isExternal
     */
    TransactionEntryDraft::TransactionEntryDraft(
        AccountId            accountId,
        finance::Cash        cash,
        TransactionEntryType type,
        bool                 isExternal
    )
        : _accountId(accountId),
          _cash(cash),
          _isExternal(isExternal),
          _type(type)
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
     * @brief get whether this transaction entry draft is for an external
     * account
     *
     * @return bool
     */
    bool TransactionEntryDraft::isExternal() const { return _isExternal; }

    /**
     * @brief get the type of this transaction entry draft
     *
     * @return TransactionEntryType
     */
    TransactionEntryType TransactionEntryDraft::getType() const
    {
        return _type;
    }

    /**
     * @brief Get the currency associated with this transaction entry draft.
     *
     * @return Currency The currency associated with this transaction entry
     * draft.
     */
    Currency TransactionEntryDraft::getCurrency() const
    {
        return _cash.getCurrency();
    }

    /**
     * @brief Construct a new Transaction Overview Draft:: Transaction Overview
     * Draft object
     *
     * @param type
     * @param timestamp
     * @param entries
     * @param legs
     * @param comment
     */
    TransactionOverviewDraft::TransactionOverviewDraft(
        TransactionDataType                type,
        Timestamp                          timestamp,
        std::vector<TransactionEntryDraft> entries,
        std::vector<TradeLegDraft>         legs,
        std::optional<std::string>         comment
    )
        : _type(type),
          _timestamp(timestamp),
          _entries(std::move(entries)),
          _legs(std::move(legs)),
          _comment(std::move(comment))
    {
    }

    /**
     * @brief get the type of the transaction overview draft
     *
     * @return TransactionDataType
     */
    TransactionDataType TransactionOverviewDraft::getType() const
    {
        return _type;
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
     * @brief Gets the legs of the transaction overview draft.
     *
     * @return const std::vector<TradeLegDraft>& The legs of the transaction
     * overview draft.
     */
    const std::vector<TradeLegDraft>& TransactionOverviewDraft::getLegs() const
    {
        return _legs;
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

    /**
     * @brief get the total fees of the transaction overview draft
     *
     * @return finance::Cash
     */
    finance::Cash TransactionOverviewDraft::getTotalFees() const
    {
        auto totalFees = finance::Cash(getCurrency());
        for (const auto& entry : _entries)
        {
            if (entry.getType() == TransactionEntryType::Fees &&
                !entry.isExternal())
            {
                totalFees += entry.getCash();
            }
        }
        return totalFees;
    }

    /**
     * @brief Get the currency associated with this transaction overview draft.
     *
     * @return Currency The currency associated with this transaction overview
     * draft.
     */
    Currency TransactionOverviewDraft::getCurrency() const
    {
        if (!_entries.empty())
            return _entries.front().getCurrency();

        if (!_legs.empty())
            return _legs.front().getCurrency();

        return Currency();
    }

    /**
     * @brief Get the account ID associated with the first leg of the
     * transaction overview draft.
     *
     * @return AccountId The account ID associated with the first leg of the
     * transaction overview draft.
     */
    AccountId TransactionOverviewDraft::getLegAccount() const
    {
        if (!_legs.empty())
            return _legs.front().getAccountId();

        return AccountId::invalid();
    }

    /**
     * @brief Get the account ID associated with the first entry of the
     * transaction overview draft.
     *
     * @return AccountId The account ID associated with the first entry of the
     * transaction overview draft.
     */
    AccountId TransactionOverviewDraft::getEntryAccountId(
        bool includeExternal
    ) const
    {
        for (const auto& entry : _entries)
            if (!entry.isExternal() || includeExternal)
                return entry.getAccountId();

        return AccountId::invalid();
    }

}   // namespace drafts