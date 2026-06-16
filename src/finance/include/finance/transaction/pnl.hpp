#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__

#include <vector>

#include "finance/transaction/transactions.hpp"
#include "utils/cash.hpp"
#include "utils/percentage.hpp"

namespace finance
{
    /**
     * @brief Base class for calculating profit and loss (PnL) for financial
     * transactions.
     */
    class PnL
    {
       private:
        /// The current price of the security, if available
        std::optional<Cash> _currentPrice = std::nullopt;
        /// The currency of the PnL calculations
        Currency _currency = Currency::Unknown;
        /// The quantity of the security involved in the transactions
        Quantity _quantity{0};
        /// The total cost of the security, calculated based on the transactions
        Cash _totalCost;
        /// The realized PnL of the security, calculated based on the
        /// transactions
        Cash _realizedPnL;
        /// The realized cost basis of the security, calculated based on the
        /// transactions
        Cash _realizedCostBasis;
        /// The total fees associated with the transactions
        Cash _fees;

       protected:
        void setCurrency(Currency currency);
        void setQuantity(Quantity quantity);
        void setTotalCost(const Cash& totalCost);
        void setRealizedPnL(const Cash& realizedPnL);
        void setRealizedCostBasis(const Cash& realizedCostBasis);
        void setFees(const Cash& fees);

        [[nodiscard]] Currency            getCurrency() const;
        [[nodiscard]] std::optional<Cash> getCurrentPrice() const;

       public:
        PnL()          = default;
        virtual ~PnL() = default;

        virtual void calculatePnL(StockTransactions& transactions)  = 0;
        virtual void calculatePnL(OptionTransactions& transactions) = 0;

        [[nodiscard]] Quantity     getQuantity() const;
        [[nodiscard]] virtual Cash getAverageCost() const;
        [[nodiscard]] Cash         getCostBasis() const;
        [[nodiscard]] Cash         getMarketValue() const;
        [[nodiscard]] Cash         getTotalPnL() const;
        [[nodiscard]] Cash         getRealizedPnL() const;
        [[nodiscard]] virtual Cash getUnrealizedPnL() const;
        [[nodiscard]] Percentage   getUnrealizedPnLPercentage() const;
        [[nodiscard]] Percentage   getRealizedPnLPercentage() const;

        void setCurrentPrice(const Cash& price);
    };

    /**
     * @brief Class for calculating PnL using the average cost method for
     * stocks.
     */
    class PnLAvg : public PnL
    {
       public:
        using PnL::PnL;
        void calculatePnL(StockTransactions& transactions) override;
        void calculatePnL(OptionTransactions& /*transactions*/) override {}
    };

    /**
     * @brief Base class for option PnL calculations, extends PnL with
     * option-specific fields and overrides.
     */
    class PnLOption : public PnL
    {
       public:
        /// Represents a single open option leg
        struct OpenLeg
        {
            OptionType    optionType;
            OptionBuySell buySell;
            Cash          strikePrice;
            Quantity      qty;
        };

        [[nodiscard]] Cash getUnrealizedPnL() const override;

        void setCurrentUnderlyingPrice(const Cash& price);

       protected:
        void setUnrealizedPnL(const Cash& unrealizedPnL);
        void setContractSize(std::int64_t contractSize);
        void setOpenLegs(std::vector<OpenLeg> legs);

        [[nodiscard]] std::int64_t getContractSize() const;

       private:
        Cash                 _unrealizedPnL;
        std::optional<Cash>  _currentUnderlyingPrice;
        std::int64_t         _contractSize{0};
        std::vector<OpenLeg> _openLegs;
    };

    /**
     * @brief Class for calculating option PnL using the average cost method.
     */
    class PnLAvgOption : public PnLOption
    {
       public:
        void calculatePnL(StockTransactions& /*transactions*/) override {}
        void calculatePnL(OptionTransactions& transactions) override;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__