#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCK_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCK_HPP__

#include <expected>
#include <string>

#include "config/id_types.hpp"
#include "utils/finance.hpp"
#include "utils/result/error.hpp"

namespace finance
{
    class YahooFinanceError;   // forward declaration
    struct TickerInfo;         // forward declaration

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
        explicit Stock(
            std::string ticker,
            Currency    currency,
            std::string shortName,
            std::string longName,
            std::string exchange,
            std::string industry,
            std::string sector,
            AssetClass  assetClass
        );

        [[nodiscard]]
        static MTResult<Stock> retrieveTickerInfo(const std::string& ticker);

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

        [[nodiscard]] std::string toString() const;

       private:
        explicit Stock(const TickerInfo& info);
    };

    /**
     * @brief A struct representing the result of inserting a stock into the
     * database, this includes the stock ID and the instrument ID that was
     * generated for the stock.
     *
     */
    struct [[nodiscard]] StockInsertionResult
    {
        /// The ID of the stock that was inserted
        StockId stockId;
        /// The ID of the instrument that was generated for the stock
        InstrumentId instrumentId;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCK_HPP__