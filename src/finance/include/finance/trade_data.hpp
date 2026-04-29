#ifndef __FINANCE__INCLUDE__FINANCE__TRADE_DATA_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRADE_DATA_HPP__

#include <vector>

#include "config/id_types.hpp"
#include "finance/cash.hpp"

namespace finance
{

    class TradeLeg
    {
       private:
        TradeLegId _id;

        AccountId _accountId;

        InstrumentId _instrumentId;

        int _quantity;

        Cash _unitPrice;

       public:
        TradeLeg(
            TradeLegId   id,
            AccountId    accountId,
            InstrumentId instrumentId,
            int          quantity,
            const Cash&  unitPrice
        );
    };

    class TradeData
    {
       private:
        std::vector<TradeLeg> _legs;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRADE_DATA_HPP__
