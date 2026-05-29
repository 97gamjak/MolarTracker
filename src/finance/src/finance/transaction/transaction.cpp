#include "finance/transaction/transaction.hpp"

#include <utility>
#include <variant>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/transaction/trade_data.hpp"
#include "finance/transaction/transaction_data.hpp"

namespace finance
{

    /**
     * @brief Construct a DomainTransaction object
     *
     * @param id
     * @param timestamp
     * @param status
     * @param data
     * @param entries
     * @param comment
     */
    DomainTransaction::DomainTransaction(
        TransactionId                 id,
        Timestamp                     timestamp,
        TransactionStatus             status,
        TransactionData               data,
        std::vector<TransactionEntry> entries,
        std::optional<std::string>    comment
    )
        : Transaction(id, timestamp, status, std::move(comment)),
          _data(std::move(data)),
          _entries(std::move(entries))
    {
    }

    /**
     * @brief Get a string representation of the transaction, this is used for
     * logging and debugging purposes
     *
     * @return std::string
     */
    std::string DomainTransaction::toString() const
    {
        std::string result = Transaction::toString();

        // clang-format off
        result += "DomainTransaction {\n";
        if (std::holds_alternative<CashData>(_data))
            result += "CashData\n";
        else if (std::holds_alternative<TradeData>(_data))
            result += "TradeData\n";
        for (const auto& leg : getLegs())
            result += "    - Leg: " + leg.toString() + "\n";

        result += "  Entries:\n";
        for (const auto& entry : _entries)
            result += "    - " + entry.toString() + "\n";
        result += "}";
        // clang-format on

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
    std::vector<TradeLeg> DomainTransaction::getLegs() const
    {
        if (std::holds_alternative<TradeData>(_data))
            return std::get<TradeData>(_data).getLegs();

        return {};
    }

    /**
     * @brief Gets the entries associated with the transaction.
     *
     * @return const std::vector<TransactionEntry>& The entries of the
     * transaction.
     */
    const std::vector<TransactionEntry>& DomainTransaction::getEntries() const
    {
        return _entries;
    }

    /**
     * @brief Gets the entries associated with the transaction.
     *
     * @return std::vector<TransactionEntry>& The entries of the
     * transaction.
     */
    std::vector<TransactionEntry>& DomainTransaction::getEntries()
    {
        return _entries;
    }

    /**
     * @brief Adds an entry to the transaction.
     *
     * @param entry The TransactionEntry to add.
     */
    void DomainTransaction::addEntry(const TransactionEntry& entry)
    {
        _entries.push_back(entry);
    }

    /**
     * @brief Gets the type of the transaction based on its data.
     *
     * @return TransactionDataType The type of the transaction (e.g., Cash,
     * Trade).
     */
    TransactionDataType DomainTransaction::getType() const
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
    const TransactionData& DomainTransaction::getData() const { return _data; }

    /**
     * @brief Gets the data associated with the transaction.
     *
     * @return TransactionData& The data of the transaction.
     */
    TransactionData& DomainTransaction::getData() { return _data; }

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
    std::vector<InstrumentId> DomainTransaction::getInstrumentIds() const
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
    Cash DomainTransaction::calculateTotalSum() const
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
    Quantity DomainTransaction::calculateTotalQuantity() const
    {
        return getTotalQuantity(_data);
    }

    /**
     * @brief Adds a leg to the transaction.
     *
     * @param leg The trade leg to add.
     */
    void DomainTransaction::addLeg(const TradeLeg& leg)
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