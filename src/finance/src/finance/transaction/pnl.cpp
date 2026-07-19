#include "finance/transaction/pnl.hpp"

#include "error/finance_error.hpp"
#include "utils/cash.hpp"
#include "utils/finance.hpp"

namespace finance
{

    [[nodiscard]]
    PositionState apply(PositionState state, const StockTrade& trade)
    {
        state.fees += trade.fees;

        if (trade.quantity.isZero())
            return state;

        const bool isStateQtyPositive = state.openQuantity > 0;
        const bool isTradeQtyPositive = trade.quantity > 0;
        const bool sameDirection      = state.openQuantity.isZero() ||
                                   (isStateQtyPositive == isTradeQtyPositive);

        if (sameDirection)
        {
            // opening or adding to the position
            state.costBasis    += trade.unitPrice * trade.quantity.abs();
            state.openQuantity += trade.quantity;
        }
        else
        {
            // reducing or closing/flipping the position
            const auto avgCost = state.costBasis / state.openQuantity.abs();
            const auto closedQty =
                std::min(trade.quantity.abs(), state.openQuantity.abs());

            state.realizedPnL       += (trade.unitPrice - avgCost) * closedQty;
            state.realizedCostBasis += avgCost * closedQty;
            state.costBasis         -= avgCost * closedQty;
            state.openQuantity -=
                (state.openQuantity > 0 ? closedQty : -closedQty);

            const auto remainder = trade.quantity.abs() - closedQty;
            if (!remainder.isZero())
            {
                // trade flips through zero: remainder opens a new position
                // in the opposite direction
                state.costBasis = trade.unitPrice * remainder;
                state.openQuantity =
                    trade.quantity > 0 ? remainder : -remainder;
            }
        }

        return state;
    }

    [[nodiscard]]
    FinanceResult<PositionState> apply(
        PositionState      state,
        const OptionTrade& trade
    )
    {
        using enum OptionType;
        using enum OptionBuySell;
        using enum TransactionOptionAction;

        if (state.contractSize.isZero())
            state.contractSize = trade.contractSize;
        else if (state.contractSize != trade.contractSize)
        {
            return FinanceError{
                FinanceErrorType::InconsistentContractSize,
                "Inconsistent contract size in option transactions"
            };
        }

        state.fees += trade.fees;

        switch (trade.action)
        {
            case Open:
            case RollOpen:
            {
                auto it = std::ranges::find_if(
                    state.openOptionLegs,
                    [&](const auto& leg)
                    {
                        return leg.type == trade.type &&
                               leg.buySell == trade.buySell &&
                               leg.strikePrice == trade.strike;
                    }
                );

                if (it != state.openOptionLegs.end())
                    it->qty += trade.quantity;
                else
                    state.openOptionLegs.push_back(
                        {trade.type,
                         trade.buySell,
                         trade.strike,
                         trade.quantity}
                    );

                if (trade.buySell == Buy)
                {
                    state.unrealizedOptionPnL -= trade.premium;
                    state.costBasis           += trade.premium;
                }
                else
                {
                    state.unrealizedOptionPnL += trade.premium;
                    state.costBasis +=
                        trade.strike * (trade.quantity * state.contractSize);
                }
                break;
            }

            case Close:
            case RollClose:
            {
                const auto openSide = (trade.buySell == Buy) ? Sell : Buy;
                auto       it       = std::ranges::find_if(
                    state.openOptionLegs,
                    [&](const auto& leg)
                    {
                        return leg.type == trade.type &&
                               leg.buySell == openSide &&
                               leg.strikePrice == trade.strike;
                    }
                );

                Cash realized, costReduction;
                if (it != state.openOptionLegs.end() && !it->qty.isZero())
                {
                    realized =
                        state.unrealizedOptionPnL / it->qty * trade.quantity;
                    costReduction = state.costBasis / it->qty * trade.quantity;

                    it->qty -= trade.quantity;
                    if (it->qty.isZero())
                        state.openOptionLegs.erase(it);
                }

                state.unrealizedOptionPnL -= realized;
                state.costBasis           -= costReduction;
                state.realizedCostBasis   += costReduction;
                state.realizedPnL         += (trade.buySell == Buy)
                                                 ? (realized - trade.premium)
                                                 : (realized + trade.premium);
                break;
            }

            case Exercised:
            {
                // TODO: needs your confirmation on exercise semantics
                break;
            }
        }

        return state;
    }

    [[nodiscard]]
    PositionState foldStockEvents(std::span<const PositionEvent> events)
    {
        PositionState state;
        for (const auto& event : events)
        {
            if (const auto* trade = std::get_if<StockTrade>(&event.data))
                state = apply(state, *trade);
        }
        return state;
    }

    /**
     * @brief Get the quantity of the security involved in the transactions
     *
     * @return Quantity
     */
    Quantity PnL::getQuantity() const { return _quantity; }

    /**
     * @brief Get the average cost of the security
     *
     * @return Cash
     */
    Cash PnL::getAverageCost() const { return _totalCost / _quantity; }

    /**
     * @brief Get the cost basis of the security, calculated based on the
     * transactions
     *
     * @return Cash
     */
    Cash PnL::getCostBasis() const { return _totalCost; }

    /**
     * @brief Set the quantity of the security involved in the transactions
     *
     * @param quantity
     */
    void PnL::setQuantity(Quantity quantity) { _quantity = quantity; }

    /**
     * @brief Set the average cost of the security
     *
     * @param totalCost The total cost to set.
     */
    void PnL::setTotalCost(const Cash& totalCost) { _totalCost = totalCost; }

    /**
     * @brief Set the realized PnL of the security
     *
     * @param realizedPnL The realized PnL to set.
     */
    void PnL::setRealizedPnL(const Cash& realizedPnL)
    {
        _realizedPnL = realizedPnL;
    }

    /**
     * @brief Set the realized cost basis of the security
     *
     * @param realizedCostBasis The realized cost basis to set.
     */
    void PnL::setRealizedCostBasis(const Cash& realizedCostBasis)
    {
        _realizedCostBasis = realizedCostBasis;
    }

    /**
     * @brief Set the fees associated with the transactions
     *
     * @param fees The fees to set.
     */
    void PnL::setFees(const Cash& fees) { _fees = fees; }

    /**
     * @brief Get the market value of the security, calculated based on the
     * current price and quantity
     *
     * @return Cash
     */
    Cash PnL::getMarketValue() const
    {
        if (!_currentPrice.has_value())
            return {_currency, 0};

        return _currentPrice.value() * _quantity;
    }

    /**
     * @brief Get the total PnL of the security, calculated based on the
     * transactions
     *
     * @return Cash
     */
    Cash PnL::getTotalPnL() const { return _realizedPnL + getUnrealizedPnL(); }

    /**
     * @brief Get the realized PnL of the security, calculated based on the
     * transactions
     *
     * @return Cash
     */
    Cash PnL::getRealizedPnL() const { return _realizedPnL; }

    /**
     * @brief Get the unrealized PnL of the security, calculated based on the
     * current price and quantity
     *
     * @return Cash
     */
    Cash PnL::getUnrealizedPnL() const
    {
        if (!_currentPrice.has_value())
            return {_currency, 0};

        return (_currentPrice.value() - getAverageCost()) * _quantity;
    }

    /**
     * @brief Get the unrealized PnL percentage of the security, calculated
     * based on the unrealized PnL and cost basis
     *
     * @return Percentage
     */
    Percentage PnL::getUnrealizedPnLPercentage() const
    {
        if (getCostBasis().isZero())
            return Percentage(0);

        return Percentage(getUnrealizedPnL() / getCostBasis());
    }

    /**
     * @brief Get the realized PnL percentage of the security, calculated based
     * on the realized PnL and realized cost basis
     *
     * @return Percentage
     */
    Percentage PnL::getRealizedPnLPercentage() const
    {
        if (_realizedCostBasis.isZero())
            return Percentage(0);

        return Percentage(_realizedPnL / _realizedCostBasis);
    }

    /**
     * @brief set the currency for the PnL calculations, this will be set based
     * on the currency of the transactions and current price
     *
     * @param currency
     */
    void PnL::setCurrency(Currency currency) { _currency = currency; }

    /**
     * @brief Get the currency of the PnL calculations, this will be set based
     * on the currency of the transactions and current price
     *
     * @return Currency
     */
    Currency PnL::getCurrency() const { return _currency; }

    /**
     * @brief Set the current price of the security, this will be used to
     * calculate the unrealized PnL based on the current market value of the
     * security.
     *
     * @param price The current price of the security.
     */
    void PnL::setCurrentPrice(const Cash& price) { _currentPrice = price; }

    /**
     * @brief calculate the PnL of the security based on the transactions for
     * the average cost method, this will calculate the average cost, realized
     * PnL, and unrealized PnL based on the transactions and current price
     *
     * @param txs
     * @return PnLResult<void>
     */
    PnLResult<void> PnLAvg::calculatePnL(StockPnLs transactions)
    {
        transactions.sort();
        Quantity quantity{0};
        Cash     fees;
        Cash     totalCost;
        Cash     realizedPnL;
        Cash     realizedCostBasis;

        for (const auto& tx : transactions)
        {
            const auto qty   = tx.quantity;
            const auto price = tx.unitPrice;

            if (getCurrency() == Currency::Unknown)
                setCurrency(price.getCurrency());

            fees     += tx.fees;
            quantity += qty;

            if (qty > 0)
            {
                totalCost += price * qty;
            }
            else
            {
                realizedPnL       += (price - getAverageCost()) * qty.abs();
                realizedCostBasis += getAverageCost() * qty.abs();
            }
        }

        setQuantity(quantity);
        setTotalCost(totalCost);
        setRealizedPnL(realizedPnL);
        setRealizedCostBasis(realizedCostBasis);
        setFees(fees);

        return {};
    }

    void PnLOption::setUnrealizedPnL(const Cash& unrealizedPnL)
    {
        _unrealizedPnL = unrealizedPnL;
    }

    void PnLOption::setContractSize(const Quantity& contractSize)
    {
        _contractSize = contractSize;
    }

    void PnLOption::setOpenLegs(std::vector<OpenLeg> legs)
    {
        _openLegs = std::move(legs);
    }

    Quantity PnLOption::getContractSize() const { return _contractSize; }

    void PnLOption::setCurrentUnderlyingPrice(const Cash& price)
    {
        _currentUnderlyingPrice = price;
    }

    Cash PnLOption::getUnrealizedPnL() const
    {
        if (!_currentUnderlyingPrice.has_value())
            return _unrealizedPnL;

        const auto& currentPrice = _currentUnderlyingPrice.value();

        Cash intrinsic{getCurrency(), 0};

        for (const auto& leg : _openLegs)
        {
            Cash legIntrinsic{getCurrency(), 0};

            switch (leg.optionType)
            {
                case OptionType::Call:
                    legIntrinsic = Cash::max(
                        Cash(getCurrency(), 0),
                        (currentPrice - leg.strikePrice) *
                            (leg.qty * _contractSize)
                    );
                    break;
                case OptionType::Put:
                    legIntrinsic = Cash::max(
                        Cash(getCurrency(), 0),
                        (leg.strikePrice - currentPrice) *
                            (leg.qty * _contractSize)
                    );
                    break;
            }

            switch (leg.buySell)
            {
                case OptionBuySell::Buy:
                    intrinsic += legIntrinsic;
                    break;
                case OptionBuySell::Sell:
                    intrinsic -= legIntrinsic;
                    break;
            }
        }

        return _unrealizedPnL + intrinsic;
    }

    PnLResult<void> PnLAvgOption::calculatePnL(OptionPnLs transactions)
    {
        transactions.sort();

        Quantity             quantity{0};
        Cash                 fees;
        Cash                 totalCost;
        Cash                 realizedPnL;
        Cash                 realizedCostBasis;
        Cash                 unrealizedPnL;
        std::vector<OpenLeg> openLegs;

        using enum OptionType;
        using enum OptionBuySell;
        using enum TransactionOptionAction;

        for (const auto& tx : transactions)
        {
            const auto oldQty = quantity;

            if (getContractSize().isZero())
                setContractSize(tx.contractSize);
            else if (getContractSize() != tx.contractSize)
            {
                return PnLError{
                    PnLErrorType::InconsistentContractSize,
                    "Inconsistent contract size in option transactions"
                };
            }

            if (getCurrency() == Currency::Unknown)
                setCurrency(tx.premium.getCurrency());

            fees += tx.fees;

            Cash realized;
            Cash costReduction;

            if (!oldQty.isZero())
            {
                realized      = unrealizedPnL / oldQty * tx.quantity;
                costReduction = totalCost / oldQty * tx.quantity;
            }

            const auto cost = tx.strike * (tx.quantity * getContractSize());

            switch (tx.action)
            {
                case Open:
                case RollOpen:
                {
                    auto it = std::ranges::find_if(
                        openLegs,
                        [&](const OpenLeg& leg)
                        {
                            return leg.optionType == tx.type &&
                                   leg.buySell == tx.buySell &&
                                   leg.strikePrice == tx.strike;
                        }
                    );

                    if (it != openLegs.end())
                        it->qty += tx.quantity;
                    else
                    {
                        openLegs.push_back(
                            {.optionType  = tx.type,
                             .buySell     = tx.buySell,
                             .strikePrice = tx.strike,
                             .qty         = tx.quantity}
                        );
                    }

                    if (tx.buySell == Buy)
                    {
                        unrealizedPnL -= tx.premium;
                        totalCost     += tx.premium;
                    }
                    else
                    {
                        unrealizedPnL += tx.premium;
                        totalCost     += cost;
                    }
                    break;
                }
                case Close:
                case RollClose:
                {
                    const auto openSide = (tx.buySell == Buy) ? Sell : Buy;
                    auto       it       = std::ranges::find_if(
                        openLegs,
                        [&](const OpenLeg& leg)
                        {
                            return leg.optionType == tx.type &&
                                   leg.buySell == openSide &&
                                   leg.strikePrice == tx.strike;
                        }
                    );

                    if (it != openLegs.end())
                    {
                        it->qty -= tx.quantity;
                        if (it->qty.isZero())
                            openLegs.erase(it);
                    }

                    unrealizedPnL     -= realized;
                    totalCost         -= costReduction;
                    realizedCostBasis += costReduction;

                    if (tx.buySell == Buy)
                        realizedPnL += (realized - tx.premium);
                    else
                        realizedPnL += (realized + tx.premium);

                    break;
                }
                case Exercised:
                {
                    const auto openSide = (tx.buySell == Buy) ? Sell : Buy;
                    auto       it       = std::ranges::find_if(
                        openLegs,
                        [&](const OpenLeg& leg)
                        {
                            return leg.optionType == tx.type &&
                                   leg.buySell == openSide &&
                                   leg.strikePrice == tx.strike;
                        }
                    );

                    if (it != openLegs.end())
                        openLegs.erase(it);

                    realizedPnL       += realized;
                    unrealizedPnL     -= realized;
                    totalCost         -= costReduction;
                    realizedCostBasis += costReduction;
                    break;
                }
            }

            quantity = Quantity{0};
            for (const auto& leg : openLegs)
                quantity += leg.qty;
        }

        setQuantity(quantity);
        setTotalCost(totalCost);
        setUnrealizedPnL(unrealizedPnL);
        setRealizedPnL(realizedPnL);
        setRealizedCostBasis(realizedCostBasis);
        setFees(fees);
        setOpenLegs(std::move(openLegs));

        return {};
    }

}   // namespace finance
