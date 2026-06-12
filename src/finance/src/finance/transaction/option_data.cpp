#include "finance/transaction/option_data.hpp"

namespace finance
{
    /**
     * @brief Construct a new Option Data:: Option Data object
     *
     * @param id
     * @param buySell
     * @param action
     * @param rolledOption
     */
    OptionData::OptionData(
        TransactionOptionId                id,
        OptionBuySell                      buySell,
        TransactionOptionAction            action,
        std::optional<TransactionOptionId> rolledOption
    )
        : _id(id),
          _buySell(buySell),
          _action(action),
          _rolledOption(rolledOption)
    {
    }

    /**
     * @brief get the ID of the option contract being traded
     *
     * @return TransactionOptionId
     */
    TransactionOptionId OptionData::getId() const { return _id; }

    /**
     * @brief get the buy/sell indicator for this option transaction
     *
     * @return OptionBuySell
     */
    OptionBuySell OptionData::getBuySell() const { return _buySell; }

    /**
     * @brief get the action for this option transaction
     *
     * @return TransactionOptionAction
     */
    TransactionOptionAction OptionData::getAction() const { return _action; }

    /**
     * @brief get the rolled option for this option transaction, if any
     *
     * @return std::optional<TransactionOptionId>
     */
    std::optional<TransactionOptionId> OptionData::getRolledOption() const
    {
        return _rolledOption;
    }

}   // namespace finance