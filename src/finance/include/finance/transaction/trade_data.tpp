#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_TPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_TPP__

#include "finance/transaction/trade_data.hpp"

namespace finance
{
    /**
     * @brief Get the legs of the trade data
     *
     * @tparam Derived
     * @return TradeLegs&
     */
    template <typename Derived>
    TradeLegs& TradeData<Derived>::getLegs()
    {
        return static_cast<Derived*>(this)->_legs;
    }

    /**
     * @brief Get the legs of the trade data (const version)
     *
     * @tparam Derived
     * @return const TradeLegs&
     */
    template <typename Derived>
    const TradeLegs& TradeData<Derived>::getLegs() const
    {
        return static_cast<const Derived*>(this)->_legs;
    }

    /**
     * @brief Add a leg to the trade data
     *
     * @tparam Derived
     * @param leg
     */
    template <typename Derived>
    void TradeData<Derived>::addLeg(const TradeLeg& leg)
    {
        static_cast<Derived*>(this)->_legs.add(leg);
    }

    template <typename Derived>
    void TradeData<Derived>::setLegs(const TradeLegs& legs)
    {
        static_cast<Derived*>(this)->_legs = legs;
    }

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_DATA_TPP__