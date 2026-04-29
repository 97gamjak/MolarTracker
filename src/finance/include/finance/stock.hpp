#ifndef __FINANCE__INCLUDE__FINANCE__STOCK_HPP__
#define __FINANCE__INCLUDE__FINANCE__STOCK_HPP__

#include <string>

#include "config/finance.hpp"
#include "config/id_types.hpp"
namespace finance
{
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
        StockId _stockId;

        /// The ID of the instrument associated with this stock
        InstrumentId _instrumentId;

        /// The ticker symbol of the stock, this is a unique identifier for the
        /// stock and is used to identify the stock in financial markets.
        std::string _ticker;

        /// The currency of the stock
        Currency _currency;

       public:
        [[nodiscard]] StockId      getStockId() const;
        [[nodiscard]] InstrumentId getInstrumentId() const;
        [[nodiscard]] std::string  getTicker() const;
        [[nodiscard]] Currency     getCurrency() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__STOCK_HPP__