#include "finance/cash_transaction.hpp"

namespace finance
{
    /**
     * @brief Construct a new Cash Transaction:: Cash Transaction object
     *
     * @param cash
     */
    CashTransaction::CashTransaction(Cash cash) : _cash(cash) {}

    /**
     * @brief Gets the amount of cash involved in the transaction.
     *
     * @return micro_units The amount of cash.
     */
    [[nodiscard]] micro_units CashTransaction::getAmount() const
    {
        return _cash.getAmount();
    }

}   // namespace finance