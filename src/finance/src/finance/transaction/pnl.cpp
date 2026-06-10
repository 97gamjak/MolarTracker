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
     * @return double
     */
    double PnL::getUnrealizedPnLPercentage() const
    {
        if (getCostBasis().isZero())
            return 0;

        return (getUnrealizedPnL() / getCostBasis()) * 100;
    }

    /**
     * @brief Get the realized PnL percentage of the security, calculated based
     * on the realized PnL and realized cost basis
     *
     * @return double
     */
    double PnL::getRealizedPnLPercentage() const
    {
        if (_realizedCostBasis.isZero())
            return 0;

        return (_realizedPnL / _realizedCostBasis) * 100;
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

        for (const auto& transaction : transactions)
        {
            const auto qty    = transaction.getQuantity();
            const auto price  = transaction.getUnitPrice();
            const auto oldQty = _quantity;

            if (getCurrency() == Currency::Unknown)
                setCurrency(price.getCurrency());

            _fees     += transaction.getFees();
            _quantity += qty;

            if (qty > 0)
            {
                _averageCost =
                    (_averageCost * oldQty + price * qty) / _quantity;
            }
            else
            {
                _realizedPnL       += (price - _averageCost) * qty.abs();
                _realizedCostBasis += _averageCost * qty.abs();
            }
        }
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
