#include "finance/stock.hpp"

namespace finance
{

    /**
     * @brief Construct a new Stock object
     *
     * @param ticker The ticker symbol of the stock, this is a unique identifier
     * for the stock and is used to identify the stock in financial markets.
     * @param currency The currency of the stock, this indicates the currency in
     * which the stock is traded, and is used for financial calculations and
     * reporting.
     */
    Stock::Stock(std::string ticker, Currency currency)
        : _id(StockId::invalid()),
          _instrumentId(InstrumentId::invalid()),
          _ticker(std::move(ticker)),
          _currency(currency)
    {
    }

    /**
     * @brief Get the StockId of the stock
     *
     * @return StockId
     */
    StockId Stock::getId() const { return _id; }

    /**
     * @brief Get the InstrumentId associated with this stock
     *
     * @return InstrumentId
     */
    InstrumentId Stock::getInstrumentId() const { return _instrumentId; }

    /**
     * @brief Get the ticker symbol of the stock
     *
     * @return std::string
     */
    std::string Stock::getTicker() const { return _ticker; }

    /**
     * @brief Get the currency of the stock
     *
     * @return Currency
     */
    Currency Stock::getCurrency() const { return _currency; }

    /**
     * @brief Set the StockId of the stock
     *
     * @param stockId
     */
    void Stock::setId(StockId stockId) { _id = stockId; }

    /**
     * @brief Set the InstrumentId associated with this stock
     *
     * @param instrumentId
     */
    void Stock::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    /**
     * @brief Create a Predicate to filter stocks by ticker
     *
     * @param ticker The ticker symbol to filter by
     * @return filter::Predicate<Stock>
     */
    filter::Predicate<Stock> HasTicker(const std::string& ticker)
    {
        return filter::makePredicate<Stock>(
            [ticker](const Stock& stock) { return stock.getTicker() == ticker; }
        );
    }

}   // namespace finance