#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_DATA_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_DATA_HPP__

#include "finance/transaction/trade_data.hpp"
#include "trade_leg.hpp"

namespace finance
{
    /**
     * @brief A class representing the data for an option transaction, this
     * class extends the TradeData class to include additional fields specific
     * to option transactions, such as the ID of the option contract being
     * traded, the buy/sell indicator, the action taken on the option (e.g.,
     * open, close, roll), and any rolled options if applicable, allowing for a
     * detailed representation of option-related transactions within the
     * financial transaction system.
     *
     */
    class OptionData : public TradeData<OptionData>
    {
        friend class TradeData<OptionData>;

       private:
        /// The ID of the option contract being traded
        TransactionOptionId _id;

        /// The buy/sell indicator for this option transaction
        OptionBuySell _buySell;

        /// The action for this option transaction
        TransactionOptionAction _action;

        /// The rolled option for this option transaction, if any
        std::optional<TransactionOptionId> _rolledOption;

        /// The trade legs associated with the option transaction
        TradeLegs _legs;

       public:
        OptionData(
            TransactionOptionId                id,
            OptionBuySell                      buySell,
            TransactionOptionAction            action,
            std::optional<TransactionOptionId> rolledOption
        );

        [[nodiscard]] TransactionOptionId     getId() const;
        [[nodiscard]] OptionBuySell           getBuySell() const;
        [[nodiscard]] TransactionOptionAction getAction() const;

        [[nodiscard]]
        std::optional<TransactionOptionId> getRolledOption() const;

        [[nodiscard]] PositionId getPositionId() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__OPTION_DATA_HPP__