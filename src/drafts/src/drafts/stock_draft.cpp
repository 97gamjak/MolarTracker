#include "drafts/stock_draft.hpp"

namespace drafts
{
    /**
     * @brief Construct a new Stock Info Draft:: Stock Info Draft object
     *
     * @param ticker
     * @param shortName
     * @param longName
     * @param exchange
     * @param currency
     * @param industry
     * @param sector
     * @param assetClass
     */
    StockInfoDraft::StockInfoDraft(
        std::string ticker,
        std::string shortName,
        std::string longName,
        std::string exchange,
        Currency    currency,
        std::string industry,
        std::string sector,
        AssetClass  assetClass
    )
        : _ticker(std::move(ticker)),
          _shortName(std::move(shortName)),
          _longName(std::move(longName)),
          _exchange(std::move(exchange)),
          _currency(currency),
          _industry(std::move(industry)),
          _sector(std::move(sector)),
          _assetClass(assetClass)
    {
    }

    /**
     * @brief Get the ticker symbol.
     *
     * @return std::string
     */
    std::string StockInfoDraft::getTicker() const { return _ticker; }

    /**
     * @brief Get the short name.
     *
     * @return std::string
     */
    std::string StockInfoDraft::getShortName() const { return _shortName; }

    /**
     * @brief Get the long name.
     *
     * @return std::string
     */
    std::string StockInfoDraft::getLongName() const { return _longName; }

    /**
     * @brief Get the exchange.
     *
     * @return std::string
     */
    std::string StockInfoDraft::getExchange() const { return _exchange; }

    /**
     * @brief Get the currency.
     *
     * @return Currency
     */
    Currency StockInfoDraft::getCurrency() const { return _currency; }

    /**
     * @brief Get the industry.
     *
     * @return std::string
     */
    std::string StockInfoDraft::getIndustry() const { return _industry; }

    /**
     * @brief Get the sector.
     *
     * @return std::string
     */
    std::string StockInfoDraft::getSector() const { return _sector; }

    /**
     * @brief Get the asset class.
     *
     * @return AssetClass
     */
    AssetClass StockInfoDraft::getAssetClass() const { return _assetClass; }

}   // namespace drafts