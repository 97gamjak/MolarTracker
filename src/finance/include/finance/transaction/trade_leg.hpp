#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_LEG_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_LEG_HPP__

#include "config/id_types.hpp"
#include "finance/cash.hpp"
#include "utils/container/vector.hpp"

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
        /// The ID of the account associated with this trade leg
        AccountId _accountId;

        /// The ID of the instrument being traded in this leg
        InstrumentId _instrumentId;

        /// The quantity of the instrument being traded in this leg
        Quantity _quantity;

        /// The unit price of the instrument being traded in this leg
        Cash _unitPrice;

        /// The ID of the position associated with this trade leg
        PositionId _positionId;

       public:
        TradeLeg(
            AccountId       accountId,
            InstrumentId    instrumentId,
            const Quantity& quantity,
            const Cash&     unitPrice,
            PositionId      positionId
        );

        [[nodiscard]] Cash         getCash() const;
        [[nodiscard]] InstrumentId getInstrumentId() const;
        [[nodiscard]] AccountId    getAccountId() const;
        [[nodiscard]] Quantity     getQuantity() const;
        [[nodiscard]] Cash         getUnitPrice() const;
        [[nodiscard]] PositionId   getPositionId() const;

        void setAccountId(AccountId accountId);
        void setInstrumentId(InstrumentId instrumentId);
        void setPositionId(PositionId positionId);

        [[nodiscard]] std::string toString() const;
    };

    /**
     * @brief A class representing the data for a trade transaction
     *
     */
    class TradeLegs : public Vector<TradeLeg>
    {
       public:
        using Vector<TradeLeg>::Vector;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRADE_LEG_HPP__