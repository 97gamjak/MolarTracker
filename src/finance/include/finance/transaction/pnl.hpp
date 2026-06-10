#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__

#include "finance/cash.hpp"
#include "finance/price_quote.hpp"
#include "finance/transaction/transactions.hpp"

namespace finance
{
    /**
     * @brief Base class for calculating profit and loss (PnL) for financial
     * transactions.
     *
     */
    class PnL
    {
       private:
        std::optional<finance::Cash> _currentPrice = std::nullopt;
        Currency                     _currency     = Currency::Unknown;

       protected:
        Quantity      _quantity{0};
        finance::Cash _averageCost;
        finance::Cash _realizedPnL;
        finance::Cash _realizedCostBasis;
        finance::Cash _fees;

        void setCurrency(Currency currency);

        [[nodiscard]] Currency getCurrency() const;

       public:
        PnL()          = default;
        virtual ~PnL() = default;

        virtual void calculatePnL(StockTransactions& transactions) = 0;

        [[nodiscard]] Quantity      getQuantity() const;
        [[nodiscard]] finance::Cash getAverageCost() const;
        [[nodiscard]] finance::Cash getCostBasis() const;
        [[nodiscard]] finance::Cash getMarketValue() const;
        [[nodiscard]] finance::Cash getTotalPnL() const;
        [[nodiscard]] finance::Cash getRealizedPnL() const;
        [[nodiscard]] finance::Cash getUnrealizedPnL() const;
        [[nodiscard]] double        getUnrealizedPnLPercentage() const;
        [[nodiscard]] double        getRealizedPnLPercentage() const;

        void setCurrentPrice(const finance::Cash& price);
    };

    /**
     * @brief Class for calculating profit and loss (PnL) using the average cost
     * method.
     *
     */
    class PnLAvg : public PnL
    {
       public:
        using PnL::PnL;
        void calculatePnL(StockTransactions& transactions) override;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
