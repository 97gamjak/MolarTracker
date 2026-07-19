#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__

#include <vector>

#include "error/finance_error.hpp"
#include "utils/cash.hpp"
#include "utils/container/vector.hpp"
#include "utils/percentage.hpp"
#include "utils/quantity.hpp"
#include "utils/timestamp.hpp"

namespace finance
{

    struct StockTrade
    {
        Quantity quantity;
        Cash     unitPrice;
        Cash     fees;
    };

    struct OptionTrade
    {
        OptionType              type;
        OptionBuySell           buySell;
        TransactionOptionAction action;
        Cash                    strike;
        Quantity                quantity;
        std::int64_t            contractSize;
        Cash                    premium;
        Cash                    fees;
    };

    struct PositionEvent
    {
        Timestamp                             timestamp;
        std::variant<StockTrade, OptionTrade> data;
    };

    class PositionEvents : public Vector<PositionEvent>
    {
       public:
        using Vector<PositionEvent>::Vector;

        void sort();
    };

    struct OpenOptionLeg
    {
        OptionType    type;
        OptionBuySell buySell;
        Cash          strikePrice;
        Quantity      qty;
    };

    struct PositionState
    {
        Quantity openQuantity{0};
        Cash     costBasis;
        Cash     realizedPnL;
        Cash     realizedCostBasis;
        Cash     unrealizedOptionPnL;   // premium-based, mirrors your existing
                                        // PnLOption field
        Cash                       fees;
        Quantity                   contractSize{0};
        std::vector<OpenOptionLeg> openOptionLegs;
    };

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

        virtual PnLResult<void> calculatePnL(StockPnLs transactions)  = 0;
        virtual PnLResult<void> calculatePnL(OptionPnLs transactions) = 0;

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
        PnLResult<void> calculatePnL(StockPnLs transactions) override;
        PnLResult<void> calculatePnL(OptionPnLs /*transactions*/) override
        {
            // TODO: remove this overload
            return PnLError::NotYetImplemented();
        }
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

       private:
        Cash                 _unrealizedPnL;
        std::optional<Cash>  _currentUnderlyingPrice;
        Quantity             _contractSize;
        std::vector<OpenLeg> _openLegs;

        [[nodiscard]] Cash getUnrealizedPnL() const override;

        void setCurrentUnderlyingPrice(const Cash& price);

       protected:
        void setUnrealizedPnL(const Cash& unrealizedPnL);
        void setContractSize(const Quantity& contractSize);
        void setOpenLegs(std::vector<OpenLeg> legs);

        [[nodiscard]] Quantity getContractSize() const;
    };

    /**
     * @brief Class for calculating option PnL using the average cost method.
     */
    class PnLAvgOption : public PnLOption
    {
       public:
        PnLResult<void> calculatePnL(StockPnLs /*transactions*/) override
        {
            // TODO: remove this overload
            return PnLError::NotYetImplemented();
        }
        PnLResult<void> calculatePnL(OptionPnLs transactions) override;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__PNL_HPP__