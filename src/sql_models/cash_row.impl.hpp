#ifndef __SQL_MODELS__CASH_ROW_IMPL_HPP__
#define __SQL_MODELS__CASH_ROW_IMPL_HPP__

#include "cash_row.hpp"

/**
 * @brief Returns a tuple of references to the fields of the CashRow
 *
 * @return constexpr auto
 */
constexpr auto CashRow::fields() { return std::tie(id, currency, amount); }

/**
 * @brief Returns a tuple of references to the fields of the CashRow (const
 * version)
 *
 * @return constexpr auto
 */
constexpr auto CashRow::fields() const
{
    return std::tie(id, currency, amount);
}

#endif   // __SQL_MODELS__CASH_ROW_IMPL_HPP__