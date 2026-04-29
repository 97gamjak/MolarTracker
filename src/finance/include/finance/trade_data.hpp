#ifndef __FINANCE__INCLUDE__FINANCE__TRADE_DATA_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRADE_DATA_HPP__

#include <vector>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "finance/cash.hpp"

namespace finance
{

    /**
     * @brief A class representing a single leg of a trade, this is used to
     * represent the details of a single leg of a trade transaction, including
     * the account involved, the financial instrument being traded, the quantity
     * of the instrument, the unit price, and the currency of the trade leg.
     *
     */
    class TradeLeg
    {
       private:
        /// The ID of the trade leg
        TradeLegId _id;

        /// The ID of the account associated with this trade leg
        AccountId _accountId;

        /// The ID of the instrument being traded in this leg
        InstrumentId _instrumentId;

        /// The quantity of the instrument being traded in this leg
        micro_units _quantity;

        /// The unit price of the instrument being traded in this leg
        Cash _unitPrice;

       public:
        TradeLeg(
            TradeLegId   id,
            AccountId    accountId,
            InstrumentId instrumentId,
            micro_units  quantity,
            const Cash&  unitPrice
        );
    };

    /**
     * @brief A class representing the data for a trade transaction
     *
     */
    class TradeData
    {
       private:
        /// A list of trade legs associated with the trade transaction
        std::vector<TradeLeg> _legs;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRADE_DATA_HPP__
