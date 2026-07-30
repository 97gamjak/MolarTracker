#ifndef __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_DATA_HPP__
#define __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_DATA_HPP__

#include <variant>

#include "finance/transaction/option_data.hpp"
#include "finance/transaction/stock_data.hpp"

namespace finance
{
    struct CashData
    {
    };

    using TransactionData = std::variant<CashData, StockData, OptionData>;

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__TRANSACTION__TRANSACTION_DATA_HPP__
