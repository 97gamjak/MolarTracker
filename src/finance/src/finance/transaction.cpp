#include "finance/transaction.hpp"

#include <utility>
#include <variant>

#include "config/finance.hpp"
#include "config/id_types.hpp"

namespace finance
{

    /**
     * @brief Construct a new Transaction:: Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param comment
     */
    Transaction::Transaction(
        TransactionId                 id,
        Timestamp                     timestamp,
        TransactionStatus             status,
        TransactionData               data,
        std::vector<TransactionEntry> entries,
        std::optional<std::string>    comment
    )
        : _id(id),
          _timestamp(timestamp),
          _status(status),
          _data(std::move(data)),
          _entries(std::move(entries)),
          _comment(std::move(comment))
    {
    }

    /**
     * @brief Gets the ID of the transaction.
     *
     * @return TransactionId The ID of the transaction.
     */
    TransactionId Transaction::getId() const { return _id; }

    /**
     * @brief Gets the timestamp of the transaction.
     *
     * @return Timestamp The timestamp of the transaction.
     */
    Timestamp Transaction::getTimestamp() const { return _timestamp; }

    /**
     * @brief Gets the status of the transaction.
     *
     * @return TransactionStatus The status of the transaction.
     */
    TransactionStatus Transaction::getStatus() const { return _status; }

    /**
     * @brief Gets the comment associated with the transaction.
     *
     * @return std::optional<std::string> The comment, if it exists.
     */
    std::optional<std::string> Transaction::getComment() const
    {
        return _comment;
    }

    /**
     * @brief Gets the entries associated with the transaction.
     *
     * @return const std::vector<TransactionEntry>& The entries of the
     * transaction.
     */
    const std::vector<TransactionEntry>& Transaction::getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Gets the entries associated with the transaction.
     *
     * @return std::vector<TransactionEntry>& The entries of the
     * transaction.
     */
    std::vector<TransactionEntry>& Transaction::getEntries()
    {
        return _entries;
    }

    /**
     * @brief Sets the ID of the transaction.
     *
     * @param id The new ID to set.
     */
    void Transaction::setId(TransactionId id) { _id = id; }

    /**
     * @brief Adds an entry to the transaction.
     *
     * @param entry The TransactionEntry to add.
     */
    void Transaction::addEntry(const TransactionEntry& entry)
    {
        _entries.push_back(entry);
    }

    TransactionDataType Transaction::getType() const
    {
        struct Visitor
        {
            TransactionDataType operator()(const CashData& /*data*/) const
            {
                return TransactionDataType::Cash;
            }

            TransactionDataType operator()(const TradeData& /*data*/) const
            {
                return TransactionDataType::Trade;
            }
        };

        return std::visit(Visitor{}, _data);
    }

    Cash Transaction::calculateTotalSum() const
    {
        if (_entries.empty())
            return Cash(Currency::USD);

        Cash total(_entries.front().getCurrency());

        for (const auto& entry : _entries)
            total += entry.getCash();

        return total;
    }

}   // namespace finance