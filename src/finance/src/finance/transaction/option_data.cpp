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

    /**
     * @brief get the position ID associated with the option data, this will
     * return the position ID of the first leg of the option data, assuming that
     * all legs of the option data are associated with the same position.
     *
     * @return PositionId
     */
    PositionId OptionData::getPositionId() const
    {
        if (_legs.empty())
            return PositionId::invalid();

        return _legs[0].getPositionId();
    }

}   // namespace finance