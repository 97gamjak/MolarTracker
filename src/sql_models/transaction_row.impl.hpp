#ifndef __SQL_MODELS__TRANSACTION_ROW_IMPL_HPP__
#define __SQL_MODELS__TRANSACTION_ROW_IMPL_HPP__

#include "transaction_row.hpp"

/**
 * @brief Returns a tuple of pointers to the fields of the TransactionRow
 * struct, which can be used for various purposes such as serialization,
 * database operations, etc.
 *
 * @return constexpr auto
 */
constexpr auto TransactionRow::fields()
{
    return std::tie(id, timestamp, status, comment);
}

/**
 * @brief Returns a tuple of pointers to the fields of the TransactionRow
 * struct, which can be used for various purposes such as serialization,
 * database operations, etc. This is a const version of the fields() method.
 *
 * @return constexpr auto
 */
constexpr auto TransactionRow::fields() const
{
    return std::tie(id, timestamp, status, comment);
}

#endif   // __SQL_MODELS__TRANSACTION_ROW_IMPL_HPP__