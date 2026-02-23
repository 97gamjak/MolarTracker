#ifndef __SQL_MODELS__CASH_ACCOUNT_ROW_IMPL_HPP__
#define __SQL_MODELS__CASH_ACCOUNT_ROW_IMPL_HPP__

#include <tuple>

#include "cash_account_row.hpp"

/**
 * @brief Returns a tuple of references to the fields of the CashAccountRow
 *
 * @return constexpr auto
 */
constexpr auto CashAccountRow::fields()
{
    return std::tie(id, type, status, profileId, currency, name);
}

/**
 * @brief Returns a tuple of references to the fields of the CashAccountRow
 * (const version)
 *
 * @return constexpr auto
 */
constexpr auto CashAccountRow::fields() const
{
    return std::tie(id, type, status, profileId, currency, name);
}

#endif   // __SQL_MODELS__CASH_ACCOUNT_ROW_IMPL_HPP__