#include "finance/instrument/stock.hpp"

#include "finance/ticker_info.hpp"
#include "finance/yf_client.hpp"
#include "utils/currency.hpp"
#include "utils/finance.hpp"

namespace finance
{
    /**
     * @brief Construct a new Stock:: Stock object
     *
     * @param ticker
     * @param currency
     * @param shortName
     * @param longName
     * @param exchange
     * @param industry
     * @param sector
     * @param assetClass
     */
    Stock::Stock(
        std::string ticker,
        Currency    currency,
        std::string shortName,
        std::string longName,
        std::string exchange,
        std::string industry,
        std::string sector,
        AssetClass  assetClass
    )
        : _id(StockId::invalid()),
          _instrumentId(InstrumentId::invalid()),
          _ticker(std::move(ticker)),
          _currency(currency),
          _shortName(std::move(shortName)),
          _longName(std::move(longName)),
          _exchange(std::move(exchange)),
          _industry(std::move(industry)),
          _sector(std::move(sector)),
          _assetClass(assetClass)
    {
    }

    /**
     * @brief Retrieve ticker information for a given stock ticker.
     *
     * @param ticker The stock ticker symbol
     * @return MTResult<Stock>
     */
    MTResult<Stock> Stock::retrieveTickerInfo(const std::string& ticker)
    {
        const auto result = YahooFinanceClient::fetchTickerInfo(ticker);

        if (!result)
            return result.error();

        return Stock(result.value());
    }

    /**
     * @brief Construct a new Stock:: Stock object -- private
     *
     * @param info
     */
    Stock::Stock(const TickerInfo& info)
        : _id(StockId::invalid()),
          _instrumentId(InstrumentId::invalid()),
          _ticker(info.symbol),
          _currency(info.currency),
          _shortName(info.shortName),
          _longName(info.longName),
          _exchange(info.exchange),
          _industry(info.industry),
          _sector(info.sector),
          _assetClass(info.assetClass)
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
     * @brief Convert the stock information to a string representation
     *
     * @return std::string The string representation of the stock information
     */
    std::string Stock::toString() const
    {
        return std::format(
            "Stock{{id: {}, instrumentId: {}, ticker: {}, currency: {}, "
            "shortName: {}, longName: {}, exchange: {}, industry: {}, sector: "
            "{}, assetClass: {}}}",
            _id.toString(),
            _instrumentId.toString(),
            _ticker,
            getSymbol(_currency),
            _shortName,
            _longName,
            _exchange,
            _industry,
            _sector,
            AssetClassMeta::toString(_assetClass)
        );
    }
}   // namespace finance