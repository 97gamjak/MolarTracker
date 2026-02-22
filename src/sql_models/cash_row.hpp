#ifndef __SQL_MODELS__CASH_ROW_HPP__
#define __SQL_MODELS__CASH_ROW_HPP__

#include "config/id_types.hpp"
#include "finance/currency.hpp"
#include "orm/field.hpp"
#include "orm/fixed_string.hpp"

/**
 * @brief Represents a row in the "cash" database table
 *
 */
struct CashRow final
{
   public:   // fields
    /// The name of the database table this struct represents
    static constexpr std::string table_name = "cash";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<CashId> id;

    /// The currency field
    orm::Field<"currency", finance::Currency, orm::not_null_t> currency;

    /// The amount field, stored in micro units
    orm::Field<"amount", std::int64_t, orm::not_null_t> amount;

   public:   // methods
    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

#ifndef __SQL_MODELS__CASH_ROW_IMPL_HPP__
#include "cash_row.impl.hpp"   // IWYU pragma: keep
#endif

#endif   // __SQL_MODELS__CASH_ROW_HPP__