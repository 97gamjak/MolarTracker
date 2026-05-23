#include "finance/transaction_data.hpp"

namespace finance
{
    /**
     * @brief Visitor for extracting quantity from transaction data
     *
     */
    struct QuantityVisitor
    {
        /**
         * @brief Get the quantity of the cash data
         *
         * @return Quantity The quantity
         */
        Quantity operator()(const CashData & /*cashData*/) const
        {
            return Quantity{0};
        }

        /**
         * @brief Get the quantity of the trade data
         *
         * @param tradeData
         * @return Quantity
         */
        Quantity operator()(const TradeData &tradeData) const
        {
            return tradeData.calculateTotalQuantity();
        }
    };

    /**
     * @brief Get the Total Quantity
     *
     * @param data
     * @return Quantity
     */
    Quantity getTotalQuantity(const TransactionData &data)
    {
        return std::visit(QuantityVisitor{}, data);
    }
}   // namespace finance