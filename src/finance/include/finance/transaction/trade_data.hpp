#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_HPP__

#include "finance/transaction/trade_leg.hpp"

namespace finance
{
    /**
     * @brief A class template representing the data for a trade, this class
     * serves as a base for specific trade data types (e.g., StockData,
     * OptionData) and provides common functionality for managing trade legs
     * associated with a trade transaction, allowing for a structured
     * representation of the details of a trade within the financial transaction
     * system.
     *
     * @tparam Derived
     */
    template <typename Derived>
    class TradeData
    {
       public:
        [[nodiscard]] TradeLegs& getLegs();

        [[nodiscard]] const TradeLegs& getLegs() const;

        void addLeg(const TradeLeg& leg);
    };
}   // namespace finance

#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_TPP__
#include "finance/transaction/trade_data.tpp"
#endif

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_HPP__