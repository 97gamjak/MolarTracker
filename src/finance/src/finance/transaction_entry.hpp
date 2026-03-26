#ifndef __FINANCE__SRC__FINANCE__TRANSACTION_ENTRY_HPP__
#define __FINANCE__SRC__FINANCE__TRANSACTION_ENTRY_HPP__

#include <variant>

#include "cash_transaction.hpp"

namespace finance
{
    using TransactionEntry = std::variant<CashTransaction>;

}   // namespace finance

#endif   // __FINANCE__SRC__FINANCE__TRANSACTION_ENTRY_HPP__
