#include "finance/transaction.hpp"

#include <utility>
#include <variant>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/trade_data.hpp"

namespace finance
{

    /**
     * @brief Construct a new Transaction:: Transaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param data
     * @param entries
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

    /**
     * @brief Gets the type of the transaction based on its data.
     *
     * @return TransactionDataType The type of the transaction (e.g., Cash,
     * Trade).
     */
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

    const TransactionData& Transaction::getData() const { return _data; }
    TransactionData&       Transaction::getData() { return _data; }

    /**
     * @brief Calculates the total sum of the transaction by summing the cash
     * amounts of all entries, this is used to ensure that the transaction is
     * balanced (i.e., the total sum should be zero for a valid transaction),
     * and can be used for validation before committing the transaction to the
     * database.
     *
     * @return Cash The total sum of the transaction, calculated by summing the
     * cash amounts of all entries.
     */
    Cash Transaction::calculateTotalSum() const
    {
        if (_entries.empty())
            return Cash(Currency::USD);

        Cash total(_entries.front().getCurrency());

        for (const auto& entry : _entries)
            total += entry.getCash();

        if (std::holds_alternative<TradeData>(_data))
            for (const auto& leg : std::get<TradeData>(_data).getLegs())
                total += leg.getCash();

        return total;
    }

    void Transaction::addLeg(const TradeLeg& leg)
    {
        struct Visitor
        {
            TradeLeg leg;
            void     operator()(TradeData& data) const { data.addLeg(leg); }

            void operator()(CashData& /*data*/) const
            {
                throw std::logic_error("Cannot add legs to cash transactions");
            }
        };

        std::visit(Visitor{leg}, _data);
    }

}   // namespace finance