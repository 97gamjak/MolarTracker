#include "finance/trade_data.hpp"

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

    /**
     * @brief Gets the cash value of the trade leg.
     *
     * @return Cash The cash value of the trade leg.
     */
    Cash TradeLeg::getCash() const { return _unitPrice * _quantity; }

    /**
     * @brief Gets the instrument ID of the trade leg.
     *
     * @return InstrumentId The instrument ID of the trade leg.
     */
    InstrumentId TradeLeg::getInstrumentId() const { return _instrumentId; }

    /**
     * @brief Gets the account ID of the trade leg.
     *
     * @return AccountId The account ID of the trade leg.
     */
    AccountId TradeLeg::getAccountId() const { return _accountId; }

    /**
     * @brief Gets the quantity of the trade leg.
     *
     * @return Quantity The quantity of the trade leg.
     */
    Quantity TradeLeg::getQuantity() const { return _quantity; }

    /**
     * @brief Gets the unit price of the trade leg.
     *
     * @return Cash The unit price of the trade leg.
     */
    Cash TradeLeg::getUnitPrice() const { return _unitPrice; }

    /**
     * @brief Sets the instrument ID of the trade leg.
     *
     * @param instrumentId The new instrument ID for the trade leg.
     */
    void TradeLeg::setInstrumentId(InstrumentId instrumentId)
    {
        _instrumentId = instrumentId;
    }

    /**
     * @brief Construct a new Trade Data:: Trade Data object
     *
     * @param legs
     */
    TradeData::TradeData(std::vector<TradeLeg> legs) : _legs(std::move(legs)) {}

    /**
     * @brief Gets the legs of the trade data.
     *
     * @return const std::vector<TradeLeg>& The legs of the trade data.
     */
    const std::vector<TradeLeg>& TradeData::getLegs() const { return _legs; }

    /**
     * @brief Gets the legs of the trade data.
     *
     * @return std::vector<TradeLeg>& The legs of the trade data.
     */
    std::vector<TradeLeg>& TradeData::getLegs() { return _legs; }

    /**
     * @brief Adds a leg to the trade data.
     *
     * @param leg The trade leg to add.
     */
    void TradeData::addLeg(const TradeLeg& leg) { _legs.push_back(leg); }

}   // namespace finance