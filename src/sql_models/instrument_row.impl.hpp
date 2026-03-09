#ifndef __SQL_MODELS__INSTRUMENT_ROW_IMPL_HPP__
#define __SQL_MODELS__INSTRUMENT_ROW_IMPL_HPP__

#include "instrument_row.hpp"

/**
 * @brief Returns a tuple of references to the fields of the InstrumentRow
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto InstrumentRow::fields()
{
    return std::tie(id, kind);
}

/**
 * @brief Returns a tuple of references to the fields of the InstrumentRow
 * (const version)
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto InstrumentRow::fields() const
{
    return std::tie(id, kind);
}

/**
 * @brief Returns a tuple of references to the fields of the CashInstrumentRow
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto CashInstrumentRow::fields()
{
    return std::tie(id, currency);
}

/**
 * @brief Returns a tuple of references to the fields of the CashInstrumentRow
 * (const version)
 *
 * @return constexpr auto
 */
[[nodiscard]] constexpr auto CashInstrumentRow::fields() const
{
    return std::tie(id, currency);
}

#endif   // __SQL_MODELS__INSTRUMENT_ROW_IMPL_HPP__