#include "finance/transaction/stock_data.hpp"

namespace finance
{

    /**
     * @brief Construct a new Stock Data:: Stock Data object
     *
     * @param legs
     */
    StockData::StockData(TradeLegs legs) : _legs(std::move(legs)) {}

    /**
     * @brief Get the position ID associated with the stock data, this will
     * return the position ID of the first leg of the stock data, assuming that
     * all legs of the stock data are associated with the same position.
     *
     * @return PositionId
     */
    PositionId StockData::getPositionId() const
    {
        if (_legs.empty())
            return PositionId::invalid();

        return _legs[0].getPositionId();
    }
}   // namespace finance