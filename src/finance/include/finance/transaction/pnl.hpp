#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__

#include "finance/transaction/transactions.hpp"
#include "utils/cash.hpp"
#include "utils/percentage.hpp"

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
        std::optional<Cash> _currentPrice = std::nullopt;
        /// The currency of the PnL calculations, this will be set based on the
        Currency _currency = Currency::Unknown;

        /// The quantity of the security involved in the transactions
        Quantity _quantity{0};
        /// The average cost of the security, calculated based on the
        /// transactions
        Cash _averageCost;
        /// The cost basis of the security, calculated based on the transactions
        Cash _realizedPnL;
        /// The market value of the security, calculated based on the current
        Cash _realizedCostBasis;
        /// The total PnL of the security, calculated based on the transactions
        Cash _fees;

       protected:
        void setCurrency(Currency currency);
        void setQuantity(Quantity quantity);
        void setAverageCost(const Cash& averageCost);
        void setRealizedPnL(const Cash& realizedPnL);
        void setRealizedCostBasis(const Cash& realizedCostBasis);
        void setFees(const Cash& fees);

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

        virtual void calculatePnL(OptionTransactions& transactions) = 0;

        [[nodiscard]] Quantity   getQuantity() const;
        [[nodiscard]] Cash       getAverageCost() const;
        [[nodiscard]] Cash       getCostBasis() const;
        [[nodiscard]] Cash       getMarketValue() const;
        [[nodiscard]] Cash       getTotalPnL() const;
        [[nodiscard]] Cash       getRealizedPnL() const;
        [[nodiscard]] Cash       getUnrealizedPnL() const;
        [[nodiscard]] Percentage getUnrealizedPnLPercentage() const;
        [[nodiscard]] Percentage getRealizedPnLPercentage() const;

        void setCurrentPrice(const Cash& price);
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
        void calculatePnL(OptionTransactions& transactions) override;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
