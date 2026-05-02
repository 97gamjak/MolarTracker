#ifndef __FINANCE__INCLUDE__FINANCE__STOCK_HPP__
#define __FINANCE__INCLUDE__FINANCE__STOCK_HPP__

#include <expected>
#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "filter/predicate.hpp"

namespace http
{
    struct HttpError;   // forward declaration
}   // namespace http

namespace finance
{
    class TickerInfo;   // forward declaration

    /**
     * @brief A class representing a stock instrument, this is used to represent
     * the details of a stock financial instrument, including its ticker symbol
     * and the associated instrument ID that links it to the base instrument
     * information (e.g., currency) stored in the instrument table.
     *
     */
    class Stock
    {
       private:
        /// The ID of the stock
        StockId _id;

        /// The ID of the instrument associated with this stock
        InstrumentId _instrumentId;

        /// The ticker symbol of the stock, this is a unique identifier for the
        /// stock and is used to identify the stock in financial markets.
        std::string _ticker;

        /// The currency of the stock
        Currency _currency;

        /// Short name of the stock
        std::string _shortName;
        /// Long name of the stock
        std::string _longName;

        /// Exchange where the stock is listed
        std::string _exchange;

        /// Industry of the stock
        std::string _industry;
        /// Sector of the stock
        std::string _sector;

        /// Asset class of the stock
        AssetClass _assetClass;

       public:
        explicit Stock(std::string ticker, Currency currency);

        [[nodiscard]]
        static std::expected<Stock, http::HttpError> retrieveTickerInfo(
            const std::string& ticker
        );

        [[nodiscard]] StockId      getId() const;
        [[nodiscard]] InstrumentId getInstrumentId() const;
        [[nodiscard]] std::string  getTicker() const;
        [[nodiscard]] Currency     getCurrency() const;
        [[nodiscard]] std::string  getShortName() const;
        [[nodiscard]] std::string  getLongName() const;
        [[nodiscard]] std::string  getExchange() const;
        [[nodiscard]] std::string  getIndustry() const;
        [[nodiscard]] std::string  getSector() const;
        [[nodiscard]] AssetClass   getAssetClass() const;

        void setId(StockId stockId);
        void setInstrumentId(InstrumentId instrumentId);

       private:
        explicit Stock(const TickerInfo& info);
    };

    [[nodiscard]]
    filter::Predicate<Stock> HasTicker(const std::string& ticker);

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__STOCK_HPP__