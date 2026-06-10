#include "finance/transaction/pnl.hpp"

namespace finance
{

    Quantity      PnL::getQuantity() const { return _quantity; }
    finance::Cash PnL::getAverageCost() const { return _averageCost; }
    finance::Cash PnL::getCostBasis() const { return _averageCost * _quantity; }
    finance::Cash PnL::getMarketValue() const
    {
        if (!_currentPrice.has_value())
            return {_currency, 0};

        return _currentPrice.value() * _quantity;
    }
    finance::Cash PnL::getTotalPnL() const
    {
        return _realizedPnL + getUnrealizedPnL();
    }
    finance::Cash PnL::getRealizedPnL() const { return _realizedPnL; }
    finance::Cash PnL::getUnrealizedPnL() const
    {
        if (!_currentPrice.has_value())
            return {_currency, 0};

        return (_currentPrice.value() - _averageCost) * _quantity;
    }

    double PnL::getUnrealizedPnLPercentage() const
    {
        if (getCostBasis().isZero())
            return 0;

        return (getUnrealizedPnL() / getCostBasis()) * 100;
    }

    double PnL::getRealizedPnLPercentage() const
    {
        if (_realizedCostBasis.isZero())
            return 0;

        return (_realizedPnL / _realizedCostBasis) * 100;
    }

    void PnL::setCurrency(Currency currency) { _currency = currency; }

    Currency PnL::getCurrency() const { return _currency; }

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

    void PnL::setCurrentPrice(const finance::Cash& price)
    {
        _currentPrice = price;
    }

}   // namespace finance
