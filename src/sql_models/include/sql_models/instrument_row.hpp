#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__INSTRUMENT_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__INSTRUMENT_ROW_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"

/**
 * @brief Represents a row in the "instrument" database table, which serves as a
 * base for different types of financial instruments. This table includes a
 * primary key and a field for the kind of instrument, which can be used to
 * distinguish between different types of instruments (e.g., cash, stock, bond,
 * etc.). The specific details of each instrument type can be stored in separate
 * tables that reference this base table through foreign key relationships.
 *
 */
struct InstrumentRow : public orm::ORMModel<"instrument">
{
    /// as we have a 1:1 relationship between InstrumentRow and
    /// CashInstrumentRow, we disallow inserting an InstrumentRow without a
    /// corresponding CashInstrumentRow
    using insert_policy = orm::requires_paired_insert_t;

    /// The id field, this is the primary key of the table and is
    /// auto-incremented
    ORM_FIELD(id, IdField<InstrumentId>)

    /// The kind field, this indicates the type of instrument (e.g., cash,
    /// stock, bond, etc.) and is a required field
    ORM_FIELD(kind, Field<"kind", InstrumentKind, orm::not_null_t>)

    /// auto generate the fields() function using the ORM_FIELDS macro
    ORM_FIELDS(InstrumentRow, id, kind);

    [[nodiscard]]
    static orm::WhereExpr hasKind(InstrumentKind kind);
};

/**
 * @brief Represents a row in the "cash_instrument" database table, which is a
 * specific type of financial instrument that represents cash in a particular
 * currency. This struct includes a foreign key reference to the InstrumentRow
 * table, indicating that each cash instrument is associated with a specific
 * instrument entry.
 *
 */
struct CashInstrumentRow : public orm::ORMModel<"cash_instrument">
{
    /// as we have a 1:1 relationship between InstrumentRow and
    /// CashInstrumentRow, we disallow inserting a CashInstrumentRow without a
    /// corresponding InstrumentRow
    using insert_policy = orm::requires_paired_insert_t;

    /// The id field, this is the primary key of the table and is also a foreign
    /// key referencing the id field of the InstrumentRow table, this indicates
    /// that each cash instrument is associated with a specific instrument
    /// entry, and the deletion behavior is set to cascade, meaning that if an
    /// instrument entry is deleted, the associated cash instrument entry will
    /// also be deleted
    ORM_FIELD(
        id,
        Field<
            "id",
            InstrumentId,
            orm::primary_key_t,
            orm::foreign_key_t<
                orm::CascadeDelete,
                InstrumentRow,
                decltype(InstrumentRow::id)>>
    )

    /// The currency field, this indicates the currency of the cash instrument
    /// and is a required field, it is also unique because we want to ensure
    /// that there is only one cash instrument per currency, this allows us to
    /// easily query for the cash instrument of a specific currency
    ORM_FIELD(
        currency,
        Field<"currency", Currency, orm::not_null_t, orm::unique_t>
    )

    /// auto generate the fields() function using the ORM_FIELDS macro
    ORM_FIELDS(CashInstrumentRow, id, currency)
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__INSTRUMENT_ROW_HPP__