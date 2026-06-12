#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_DATA_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_DATA_HPP__

#include "finance/transaction/trade_data.hpp"
#include "finance/transaction/trade_leg.hpp"

namespace finance
{
    /**
     * @brief A class representing the data for a stock transaction, this class
     * extends the TradeData class to include additional fields specific to
     * stock transactions, such as the trade legs associated with the stock
     * transaction, allowing for a detailed representation of stock-related
     * trades.
     */
    class StockData : public TradeData<StockData>
    {
        friend class TradeData<StockData>;

       private:
        /// The trade legs associated with the stock transaction
        TradeLegs _legs;

       public:
        explicit StockData() = default;
        explicit StockData(TradeLegs legs);

        [[nodiscard]] PositionId getPositionId() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__STOCK_DATA_HPP__