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
          _currency(currency),
          _assetClass(AssetClass::Unknown)
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
     * @brief Get the short name of the stock
     *
     * @return std::string
     */
    std::string Stock::getShortName() const { return _shortName; }

    /**
     * @brief Get the long name of the stock
     *
     * @return std::string
     */
    std::string Stock::getLongName() const { return _longName; }

    /**
     * @brief Get the exchange of the stock
     *
     * @return std::string
     */
    std::string Stock::getExchange() const { return _exchange; }

    /**
     * @brief Get the industry of the stock
     *
     * @return std::string
     */
    std::string Stock::getIndustry() const { return _industry; }

    /**
     * @brief Get the sector of the stock
     *
     * @return std::string
     */
    std::string Stock::getSector() const { return _sector; }

    /**
     * @brief Get the asset class of the stock
     *
     * @return AssetClass
     */
    AssetClass Stock::getAssetClass() const { return _assetClass; }

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