#ifndef __SQL_MODELS__ACCOUNT_ROW_IMPL_HPP__
#define __SQL_MODELS__ACCOUNT_ROW_IMPL_HPP__

#include <tuple>

#include "account_row.hpp"

/**
 * @brief Returns a tuple of references to the fields of the AccountRow
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto AccountRow::fields() { return std::tie(id, kind); }

/**
 * @brief Returns a tuple of references to the fields of the AccountRow (const
 * version)
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto AccountRow::fields() const
{
    return std::tie(id, kind);
}

/**
 * @brief Returns a tuple of references to the fields of the CashAccountRow
 *
 * @return constexpr auto
 */
constexpr auto CashAccountRow::fields()
{
    return std::tie(id, status, profileId, currency, name);
}

/**
 * @brief Returns a tuple of references to the fields of the CashAccountRow
 * (const version)
 *
 * @return constexpr auto
 */
constexpr auto CashAccountRow::fields() const
{
    return std::tie(id, status, profileId, currency, name);
}

#endif   // __SQL_MODELS__ACCOUNT_ROW_IMPL_HPP__