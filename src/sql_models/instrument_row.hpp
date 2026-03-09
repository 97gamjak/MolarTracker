#ifndef __SQL_MODELS__INSTRUMENT_ROW_HPP__
#define __SQL_MODELS__INSTRUMENT_ROW_HPP__

#include "config/id_types.hpp"
#include "finance/currency.hpp"
#include "orm/field.hpp"

/**
 * @brief Represents a row in the "instrument" database table, which serves as a
 * base for different types of financial instruments. This table includes a
 * primary key and a field for the kind of instrument, which can be used to
 * distinguish between different types of instruments (e.g., cash, stock, bond,
 * etc.). The specific details of each instrument type can be stored in separate
 * tables that reference this base table through foreign key relationships.
 *
 */
struct InstrumentRow
{
    /// The name of the database table this struct represents
    static constexpr std::string table_name = "instrument";

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    orm::IdField<InstrumentId> id;

    /// The kind field, this indicates the type of instrument (e.g., cash,
    /// stock, bond, etc.) and is a required field
    orm::Field<"kind", InstrumentKind, orm::not_null_t> kind;

    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

/**
 * @brief Represents a row in the "cash_instrument" database table, which is a
 * specific type of financial instrument that represents cash in a particular
 * currency. This struct includes a foreign key reference to the InstrumentRow
 * table, indicating that each cash instrument is associated with a specific
 * instrument entry.
 *
 */
struct CashInstrumentRow
{
    /// The name of the database table this struct represents
    static constexpr std::string table_name = "cash_instrument";

    /// The id field, this is the primary key of the table and is also a foreign
    /// key referencing the id field of the InstrumentRow table, this indicates
    /// that each cash instrument is associated with a specific instrument
    /// entry, and the deletion behavior is set to cascade, meaning that if an
    /// instrument entry is deleted, the associated cash instrument entry will
    /// also be deleted
    orm::Field<
        "id",
        InstrumentId,
        orm::primary_key_t,
        orm::foreign_key_t<
            orm::CascadeDelete,
            InstrumentRow,
            decltype(InstrumentRow::id)>>
        id;

    /// The currency field, this indicates the currency of the cash instrument
    /// and is a required field, it is also unique because we want to ensure
    /// that there is only one cash instrument per currency, this allows us to
    /// easily query for the cash instrument of a specific currency
    orm::Field<"currency", finance::Currency, orm::not_null_t, orm::unique_t>
        currency;

    [[nodiscard]] constexpr auto fields();
    [[nodiscard]] constexpr auto fields() const;
};

#ifndef __SQL_MODELS__INSTRUMENT_ROW_IMPL_HPP__
#include "instrument_row.impl.hpp"   // IWYU pragma: keep
#endif

#endif   // __SQL_MODELS__INSTRUMENT_ROW_HPP__