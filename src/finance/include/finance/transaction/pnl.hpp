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
        /// The current price of the security, if available
        std::optional<finance::Cash> _currentPrice = std::nullopt;
        /// The currency of the PnL calculations, this will be set based on the
        Currency _currency = Currency::Unknown;

       protected:
        /// The quantity of the security involved in the transactions
        Quantity _quantity{0};
        /// The average cost of the security, calculated based on the
        /// transactions
        finance::Cash _averageCost;
        /// The cost basis of the security, calculated based on the transactions
        finance::Cash _realizedPnL;
        /// The market value of the security, calculated based on the current
        finance::Cash _realizedCostBasis;
        /// The total PnL of the security, calculated based on the transactions
        finance::Cash _fees;

        void setCurrency(Currency currency);

        [[nodiscard]] Currency getCurrency() const;

       public:
        /// Default constructor and virtual destructor for PnL
        PnL()          = default;
        virtual ~PnL() = default;

        /**
         * @brief calculate the PnL of the security based on the transactions
         *
         * @param transactions
         */
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
