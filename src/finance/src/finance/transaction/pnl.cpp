#include "finance/transaction/pnl.hpp"

namespace finance
{

    /**
     * @brief Get the quantity of the security involved in the transactions
     *
     * @return Quantity
     */
    Quantity PnL::getQuantity() const { return _quantity; }

    /**
     * @brief Get the average cost of the security
     *
     * @return finance::Cash
     */
    finance::Cash PnL::getAverageCost() const { return _averageCost; }

    /**
     * @brief Get the cost basis of the security, calculated based on the
     * transactions
     *
     * @return finance::Cash
     */
    finance::Cash PnL::getCostBasis() const { return _averageCost * _quantity; }

    /**
     * @brief Set the quantity of the security involved in the transactions
     *
     * @param quantity
     */
    void PnL::setQuantity(Quantity quantity) { _quantity = quantity; }

    /**
     * @brief Set the average cost of the security
     *
     * @param averageCost The average cost to set.
     */
    void PnL::setAverageCost(const Cash& averageCost)
    {
        _averageCost = averageCost;
    }

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
     * @return finance::Cash
     */
    finance::Cash PnL::getMarketValue() const
    {
        if (!_currentPrice.has_value())
            return {_currency, 0};

        return _currentPrice.value() * _quantity;
    }

    /**
     * @brief Get the total PnL of the security, calculated based on the
     * transactions
     *
     * @return finance::Cash
     */
    finance::Cash PnL::getTotalPnL() const
    {
        return _realizedPnL + getUnrealizedPnL();
    }

    /**
     * @brief Get the realized PnL of the security, calculated based on the
     * transactions
     *
     * @return finance::Cash
     */
    finance::Cash PnL::getRealizedPnL() const { return _realizedPnL; }

    /**
     * @brief Get the unrealized PnL of the security, calculated based on the
     * current price and quantity
     *
     * @return finance::Cash
     */
    finance::Cash PnL::getUnrealizedPnL() const
    {
        if (!_currentPrice.has_value())
            return {_currency, 0};

        return (_currentPrice.value() - _averageCost) * _quantity;
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
     * @brief calculate the PnL of the security based on the transactions for
     * the average cost method, this will calculate the average cost, realized
     * PnL, and unrealized PnL based on the transactions and current price
     *
     * @param transactions
     */
    void PnLAvg::calculatePnL(StockTransactions& transactions)
    {
        transactions.sort();
        Quantity quantity{0};
        Cash     fees;
        Cash     averageCost;
        Cash     realizedPnL;
        Cash     realizedCostBasis;

        for (const auto& transaction : transactions)
        {
            const auto qty    = transaction.getQuantity();
            const auto price  = transaction.getUnitPrice();
            const auto oldQty = quantity;

            if (getCurrency() == Currency::Unknown)
                setCurrency(price.getCurrency());

            fees     += transaction.getFees();
            quantity += qty;

            if (qty > 0)
            {
                averageCost = (averageCost * oldQty + price * qty) / quantity;
            }
            else
            {
                realizedPnL       += (price - averageCost) * qty.abs();
                realizedCostBasis += averageCost * qty.abs();
            }
        }

        setQuantity(quantity);
        setAverageCost(averageCost);
        setRealizedPnL(realizedPnL);
        setRealizedCostBasis(realizedCostBasis);
        setFees(fees);
    }

    /**
     * @brief Set the current price of the security, this will be used to
     * calculate the unrealized PnL based on the current market value of the
     * security.
     *
     * @param price The current price of the security.
     */
    void PnL::setCurrentPrice(const finance::Cash& price)
    {
        _currentPrice = price;
    }

}   // namespace finance
