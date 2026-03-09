#ifndef __SQL_MODELS__TRANSACTION_ENTRY_ROW_IMPL_HPP__
#define __SQL_MODELS__TRANSACTION_ENTRY_ROW_IMPL_HPP__

#include "transaction_entry_row.hpp"

/**
 * @brief Returns a tuple of references to the fields of the TransactionEntryRow
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto TransactionEntryRow::fields()
{
    return std::tie(id, transactionId, accountId, instrumentId, amount);
}

/**
 * @brief Returns a tuple of references to the fields of the TransactionEntryRow
 * (const version)
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto TransactionEntryRow::fields() const
{
    return std::tie(id, transactionId, accountId, instrumentId, amount);
}

#endif   // __SQL_MODELS__TRANSACTION_ENTRY_ROW_IMPL_HPP__