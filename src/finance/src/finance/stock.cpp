#include "finance/stock.hpp"

namespace finance
{
    /**
     * @brief Get the StockId of the stock
     *
     * @return StockId
     */
    StockId Stock::getStockId() const { return _stockId; }

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

}   // namespace finance