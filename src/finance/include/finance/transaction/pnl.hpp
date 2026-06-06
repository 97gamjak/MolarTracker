#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__

#include "finance/cash.hpp"
#include "finance/price_quote.hpp"
#include "finance/transaction/transactions.hpp"

namespace finance
{
    class PnL
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        Quantity      _quantity{0};
        finance::Cash _averageCost;
        finance::Cash _realizedPnL;
        finance::Cash _realizedCostBasis;
        finance::Cash _fees;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

       public:
        PnL()          = default;
        virtual ~PnL() = default;

        virtual void calculatePnL(StockTransactions& transactions) = 0;

        [[nodiscard]] Quantity      getQuantity() const;
        [[nodiscard]] finance::Cash getAverageCost() const;
        [[nodiscard]] finance::Cash getCostBasis() const;
        [[nodiscard]] finance::Cash getTotalPnL(
            const finance::PriceQuote& priceQuote
        ) const;
        [[nodiscard]] finance::Cash getRealizedPnL() const;
        [[nodiscard]] finance::Cash getUnrealizedPnL(
            const finance::PriceQuote& quote
        ) const;

        [[nodiscard]] double getUnrealizedPnLPercentage(
            const finance::PriceQuote& quote
        ) const;

        [[nodiscard]] double getRealizedPnLPercentage() const;
    };

    class PnLAvg : public PnL
    {
       public:
        using PnL::PnL;
        void calculatePnL(StockTransactions& transactions) override;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
