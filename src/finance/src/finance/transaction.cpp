#include "finance/transaction.hpp"

#include <utility>
#include <variant>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/trade_data.hpp"
#include "finance/transaction_data.hpp"

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
     * @brief Get a string representation of the transaction, this is used for
     * logging and debugging purposes
     *
     * @return std::string
     */
    std::string Transaction::toString() const
    {
        std::string result  = "Transaction {\n";
        result             += "  ID: " + _id.toString() + "\n";
        result += "  Timestamp: " + _timestamp.humanReadable() + "\n";
        result +=
            "  Status: " + std::to_string(static_cast<int>(_status)) + "\n";
        result += "  Data: ";
        if (std::holds_alternative<CashData>(_data))
            result += "CashData\n";
        else if (std::holds_alternative<TradeData>(_data))
            result += "TradeData\n";
        for (const auto& leg : getLegs())
            result += "    - Leg: " + leg.toString() + "\n";

        result += "  Entries:\n";
        for (const auto& entry : _entries)
            result += "    - " + entry.toString() + "\n";
        if (_comment.has_value())
            result += "  Comment: " + _comment.value() + "\n";
        result += "}";

        return result;
    }

    /**
     * @brief Get the legs of the transaction, this is used to get the trade
     * legs associated with the transaction, which contain information about
     * the instruments being traded, the quantities, and the unit prices.
     *
     * @return std::vector<TradeLeg> A vector of TradeLeg objects representing
     * the legs of the transaction. If the transaction does not contain trade
     * data, an empty vector is returned.
     */
    std::vector<TradeLeg> Transaction::getLegs() const
    {
        if (std::holds_alternative<TradeData>(_data))
            return std::get<TradeData>(_data).getLegs();

        return {};
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

    /**
     * @brief Gets the data associated with the transaction.
     *
     * @return const TransactionData& The data of the transaction.
     */
    const TransactionData& Transaction::getData() const { return _data; }

    /**
     * @brief Gets the data associated with the transaction.
     *
     * @return TransactionData& The data of the transaction.
     */
    TransactionData& Transaction::getData() { return _data; }

    /**
     * @brief Gets the instrument IDs associated with the transaction, this is
     * used to determine which instruments are involved in the transaction, and
     * can be useful for various operations such as filtering transactions by
     * instrument or analyzing the instruments involved in a set of
     * transactions.
     *
     * @return std::vector<InstrumentId> A vector of instrument IDs associated
     * with the transaction, this includes all instruments that are part of the
     * transaction's data (e.g., trade legs) and any relevant entries.
     */
    std::vector<InstrumentId> Transaction::getInstrumentIds() const
    {
        return std::visit(
            GetIdVisitor<InstrumentId, decltype(&TradeLeg::getInstrumentId)>{
                &TradeLeg::getInstrumentId
            },
            _data
        );
    }

    /**
     * @brief Calculates the total sum of the transaction by summing the
     * cash amounts of all entries, this is used to ensure that the
     * transaction is balanced (i.e., the total sum should be zero for a
     * valid transaction), and can be used for validation before committing
     * the transaction to the database.
     *
     * @return Cash The total sum of the transaction, calculated by summing
     * the cash amounts of all entries.
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

    /**
     * @brief Calculates the total quantity of the transaction.
     *
     * @return Quantity
     */
    Quantity Transaction::calculateTotalQuantity() const
    {
        return getTotalQuantity(_data);
    }

    /**
     * @brief Adds a leg to the transaction.
     *
     * @param leg The trade leg to add.
     */
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

    /**
     * @brief Construct a new Transactions:: Transactions object
     *
     * @param transactions
     */
    Transactions::Transactions(const std::vector<Transaction>& transactions)
        : _transactions(transactions)
    {
    }

    /**
     * @brief Calculates the total quantity of all transactions.
     *
     * @return Quantity The total quantity of all transactions.
     */
    Quantity Transactions::calculateTotalQuantity() const
    {
        Quantity total{0};
        for (const auto& transaction : _transactions)
            total += transaction.calculateTotalQuantity();

        return total;
    }

    /**
     * @brief Gets the instrument IDs associated with all transactions.
     *
     * @return std::vector<InstrumentId> A vector of instrument IDs associated
     * with all transactions.
     */
    std::vector<InstrumentId> Transactions::getInstrumentIds() const
    {
        std::vector<InstrumentId> ids;
        for (const auto& transaction : _transactions)
        {
            const auto txIds = transaction.getInstrumentIds();
            ids.insert(ids.end(), txIds.begin(), txIds.end());
        }
        return ids;
    }

}   // namespace finance