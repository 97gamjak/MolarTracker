#include "finance/transaction/trade_leg.hpp"

#include <format>

namespace finance
{

    /**
     * @brief Construct a new Trade Leg:: Trade Leg object
     *
     * @param accountId The ID of the account associated with this trade leg
     * @param instrumentId The ID of the instrument being traded in this leg
     * @param quantity The quantity of the instrument being traded in this leg
     * @param amount The cash amount of the instrument being traded in this
     * leg
     * @param positionId The ID of the position associated with this trade leg
     */
    TradeLeg::TradeLeg(
        AccountId       accountId,
        InstrumentId    instrumentId,
        const Quantity& quantity,
        const Cash&     amount,
        PositionId      positionId
    )
        : _accountId(accountId),
          _instrumentId(instrumentId),
          _quantity(quantity),
          _amount(amount),
          _positionId(positionId)
    {
    }

    /**
     * @brief get a string representation of the trade leg, this is used for
     * logging and debugging purposes
     *
     * @return std::string
     */
    std::string TradeLeg::toString() const
    {
        return std::format(
            "TradeLeg(AccountId: {}, InstrumentId: {}, PositionId: {}, "
            "Quantity: {}, UnitPrice: {})",
            _accountId.toString(),
            _instrumentId.toString(),
            _positionId.toString(),
            _quantity.toString(),
            _amount.toString()
        );
    }

    /**
     * @brief Gets the cash value of the trade leg.
     *
     * @return Cash The cash value of the trade leg.
     */
    Cash TradeLeg::getCash() const { return _amount * _quantity; }

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
     * @brief Gets the cash amount of the trade leg.
     *
     * @return Cash The cash amount of the trade leg.
     */
    Cash TradeLeg::getAmount() const { return _amount; }

    /**
     * @brief Gets the position ID of the trade leg.
     *
     * @return PositionId The position ID of the trade leg.
     */
    PositionId TradeLeg::getPositionId() const { return _positionId; }

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
     * @brief Sets the position ID of the trade leg.
     *
     * @param positionId The new position ID for the trade leg.
     */
    void TradeLeg::setPositionId(PositionId positionId)
    {
        _positionId = positionId;
    }

    /**
     * @brief Sets the account ID of the trade leg.
     *
     * @param accountId The new account ID for the trade leg.
     */
    void TradeLeg::setAccountId(AccountId accountId) { _accountId = accountId; }

}   // namespace finance