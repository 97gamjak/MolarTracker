#include "finance/trade_data.hpp"

#include "config/finance.hpp"

namespace finance
{

    /**
     * @brief Construct a new Trade Leg:: Trade Leg object
     *
     * @param accountId The ID of the account associated with this trade leg
     * @param instrumentId The ID of the instrument being traded in this leg
     * @param quantity The quantity of the instrument being traded in this leg
     * @param unitPrice The unit price of the instrument being traded in this
     * leg
     */
    TradeLeg::TradeLeg(
        AccountId       accountId,
        InstrumentId    instrumentId,
        const Quantity& quantity,
        const Cash&     unitPrice
    )
        : _accountId(accountId),
          _instrumentId(instrumentId),
          _quantity(quantity),
          _unitPrice(unitPrice)
    {
    }

    Cash TradeLeg::getCash() const { return _unitPrice * _quantity; }

    InstrumentId TradeLeg::getInstrumentId() const { return _instrumentId; }

    AccountId TradeLeg::getAccountId() const { return _accountId; }

    Quantity TradeLeg::getQuantity() const { return _quantity; }

    Cash TradeLeg::getUnitPrice() const { return _unitPrice; }

    void TradeLeg::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    TradeData::TradeData(std::vector<TradeLeg> legs) : _legs(std::move(legs)) {}

    const std::vector<TradeLeg>& TradeData::getLegs() const { return _legs; }

    std::vector<TradeLeg>& TradeData::getLegs() { return _legs; }

    void TradeData::addLeg(const TradeLeg& leg) { _legs.push_back(leg); }
}   // namespace finance