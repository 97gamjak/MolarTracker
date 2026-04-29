#include "finance/trade_data.hpp"

#include "config/finance.hpp"

namespace finance
{

    /**
     * @brief Construct a new Trade Leg:: Trade Leg object
     *
     * @param id The ID of the trade leg
     * @param accountId The ID of the account associated with this trade leg
     * @param instrumentId The ID of the instrument being traded in this leg
     * @param quantity The quantity of the instrument being traded in this leg
     * @param unitPrice The unit price of the instrument being traded in this
     * leg
     */
    TradeLeg::TradeLeg(
        TradeLegId   id,
        AccountId    accountId,
        InstrumentId instrumentId,
        micro_units  quantity,
        const Cash&  unitPrice
    )
        : _id(id),
          _accountId(accountId),
          _instrumentId(instrumentId),
          _quantity(quantity),
          _unitPrice(unitPrice)
    {
    }

}   // namespace finance