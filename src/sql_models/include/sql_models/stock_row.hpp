#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__STOCK_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__STOCK_ROW_HPP__

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "sql_models/instrument_row.hpp"

/**
 * @brief Represents a row in the "stock_instrument" database table, which
 * contains specific details about stock instruments. This table has a
 * one-to-one relationship with the "instrument" table, where each row in the
 * "stock_instrument" table corresponds to a row in the "instrument" table. The
 * primary key of this table is also a foreign key referencing the id field of
 * the "instrument" table, ensuring that each stock instrument is associated
 * with a valid instrument entry.
 *
 */
struct StockRow : public orm::ORMModel<"stock">
{
    /// The ID of the stock instrument, this is the primary key for the stock
    /// table and is also a foreign key referencing the id field of the
    /// instrument table, ensuring a one-to-one relationship between the two
    /// tables.
    ORM_FIELD(id, IdField<StockId>)

    /// The ticker symbol of the stock, this is a unique identifier for the
    /// stock and is used to identify the stock in financial markets.
    ORM_FIELD(
        ticker,
        Field<"ticker", std::string, orm::not_null_t, orm::unique_t>
    )

    /// The ID of the instrument associated with this stock, this is a foreign
    /// key referencing the id field of the instrument table, and is used to
    /// associate this stock instrument with a specific entry in the instrument
    /// table, allowing for a structured representation of different types of
    /// financial instruments while still maintaining a common base for all
    /// instruments.
    ORM_FIELD(
        instrumentId,
        InstrumentRow::template ForeignId<orm::CascadeDelete>
    )

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(StockRow, id, ticker, instrumentId);
    /// @endcond
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__STOCK_ROW_HPP__