#include "finance/transaction/pnl.hpp"

#include "finance/price_quote.hpp"

namespace finance
{

    Quantity      PnL::getQuantity() const { return _quantity; }
    finance::Cash PnL::getAverageCost() const { return _averageCost; }
    finance::Cash PnL::getCostBasis() const { return _averageCost * _quantity; }
    finance::Cash PnL::getTotalPnL(const finance::PriceQuote& priceQuote) const
    {
        return _realizedPnL + getUnrealizedPnL(priceQuote);
    }
    finance::Cash PnL::getRealizedPnL() const { return _realizedPnL; }
    finance::Cash PnL::getUnrealizedPnL(
        const finance::PriceQuote& priceQuote
    ) const
    {
        return (priceQuote.getPrice() - _averageCost) * _quantity;
    }

    double PnL::getUnrealizedPnLPercentage(
        const finance::PriceQuote& priceQuote
    ) const
    {
        if (getCostBasis().isZero())
            return 0;

        return (getUnrealizedPnL(priceQuote) / getCostBasis()) * 100;
    }

    double PnL::getRealizedPnLPercentage() const
    {
        return _realizedPnL / _realizedCostBasis * 100;
    }

    void PnLAvg::calculatePnL(StockTransactions& transactions)
    {
        transactions.sort();

        for (const auto& transaction : transactions)
        {
            const auto qty    = transaction.getQuantity();
            const auto oldQty = _quantity;
            const auto price  = transaction.getUnitPrice();

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

}   // namespace finance
