#include "finance/transaction/domain_transaction.hpp"

#include <algorithm>
#include <utility>
#include <variant>

#include "config/id_types.hpp"
#include "finance/transaction/stock_data.hpp"
#include "finance/transaction/transaction_data.hpp"
#include "utils/finance.hpp"

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
        TransactionId              id,
        Timestamp                  timestamp,
        TransactionStatus          status,
        TransactionData            data,
        TransactionEntries         entries,
        std::optional<std::string> comment
    )
        : BaseTransaction(id, timestamp, status, std::move(comment)),
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
        std::string result = BaseTransaction::toString();

        // clang-format off
        result += "DomainTransaction {\n";
        result += TransactionDataTypeMeta::toString(getType()) + "\n";
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
     * @return TradeLegs A vector of TradeLeg objects
     * representing the legs of the transaction. If the transaction does not
     * contain trade data, an empty vector is returned.
     */
    const TradeLegs& DomainTransaction::getLegs() const
    {
        switch (getType())
        {
            case TransactionDataType::Stock:
                return std::get<StockData>(_data).getLegs();
            case TransactionDataType::Option:
                return std::get<OptionData>(_data).getLegs();
            case TransactionDataType::Cash:
            {
                static const TradeLegs emptyLegs;
                return emptyLegs;
            }
        }

        std::unreachable();
    }

    /**
     * @brief Gets the entries associated with the transaction.
     *
     * @return const TransactionEntries& The entries of the
     * transaction.
     */
    const TransactionEntries& DomainTransaction::getEntries() const
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
        _entries.add(entry);
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

            TransactionDataType operator()(const StockData& /*data*/) const
            {
                return TransactionDataType::Stock;
            }

            TransactionDataType operator()(const OptionData& /*data*/) const
            {
                return TransactionDataType::Option;
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
     * @brief Adds a leg to the transaction.
     *
     * @param leg The trade leg to add.
     */
    void DomainTransaction::addLeg(const TradeLeg& leg)
    {
        switch (getType())
        {
            case TransactionDataType::Stock:
            {
                auto& data = std::get<StockData>(_data);
                data.addLeg(leg);
                break;
            }
            case TransactionDataType::Option:
            {
                auto& data = std::get<OptionData>(_data);
                data.addLeg(leg);
                break;
            }
            case TransactionDataType::Cash:
                throw std::logic_error(
                    "Cannot add trade legs to cash transactions"
                );
        }
    }

    void DomainTransaction::setLegs(const TradeLegs& legs)
    {
        switch (getType())
        {
            case TransactionDataType::Stock:
            {
                auto& data = std::get<StockData>(_data);
                data.setLegs(legs);
                break;
            }
            case TransactionDataType::Option:
            {
                auto& data = std::get<OptionData>(_data);
                data.setLegs(legs);
                break;
            }
            case TransactionDataType::Cash:
                throw std::logic_error(
                    "Cannot set trade legs on cash transactions"
                );
        }
    }

    void DomainTransaction::setEntries(const TransactionEntries& entries)
    {
        _entries = entries;
    }

    /**
     * @brief Checks if the transaction has a specific position ID
     *
     * @param id
     * @return true
     * @return false
     */
    bool DomainTransaction::hasPositionId(PositionId id) const
    {
        switch (getType())
        {
            case TransactionDataType::Stock:
                return std::get<StockData>(_data).getPositionId() == id;
            case TransactionDataType::Option:
                return std::get<OptionData>(_data).getPositionId() == id;
            case TransactionDataType::Cash:
                return false;
        }

        std::unreachable();
    }

    /**
     * @brief Checks if the transaction has a specific instrument ID
     *
     * @param id
     * @return true
     * @return false
     */
    bool DomainTransaction::hasInstrumentId(InstrumentId id) const
    {
        return std::ranges::any_of(
            getLegs(),
            [id](const auto& leg) { return leg.getInstrumentId() == id; }
        );
    }

}   // namespace finance