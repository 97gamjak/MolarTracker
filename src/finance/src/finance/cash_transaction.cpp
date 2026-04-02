#include "finance/cash_transaction.hpp"

namespace finance
{
    /**
     * @brief Construct a new Cash Transaction:: Cash Transaction object
     *
     * @param cash
     */
    CashTransaction::CashTransaction(Cash cash) : _cash(cash) {}

    [[nodiscard]] micro_units CashTransaction::getAmount() const
    {
        return _cash.getAmount();
    }

}   // namespace finance