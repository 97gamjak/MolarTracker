#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__STOCK_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__STOCK_ROW_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "orm/where_expr.hpp"
#include "sql_models/instrument_row.hpp"

/**
 * @brief Represents a row in the "stock" database table, which
 * contains specific details about stock instruments. This table has a
 * one-to-one relationship with the "instrument" table, where each row in the
 * "stock" table corresponds to a row in the "instrument" table. The
 * primary key of this table is also a foreign key referencing the id field of
 * the "instrument" table, ensuring that each stock instrument is associated
 * with a valid instrument entry.
 *
 */
struct StockRow : public orm::ORMModel<"stock">
{
    [[nodiscard]]
    static orm::WhereExpr hasTicker(const std::string& ticker);

    /// The ID of the stock instrument, this is the primary key for the stock
    /// table.
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

    /// The currency of the stock, this indicates the currency in which the
    /// stock is traded, and is used for financial calculations and reporting.
    ORM_FIELD(currency, Field<"currency", Currency, orm::not_null_t>)

    /// The short name of the stock, this is a brief identifier for the stock.
    ORM_FIELD(shortName, Field<"short_name", std::string, orm::not_null_t>)

    /// The long name of the stock, this is a more descriptive identifier for
    /// the stock.
    ORM_FIELD(longName, Field<"long_name", std::string, orm::not_null_t>)

    /// The asset class of the stock, this indicates the category of the stock
    ORM_FIELD(assetClass, Field<"asset_class", AssetClass, orm::not_null_t>)

    /// The sector of the stock, this indicates the industry sector to which the
    /// stock belongs.
    ORM_FIELD(sector, Field<"sector", std::string, orm::not_null_t>)

    /// The industry of the stock, this indicates the specific industry to which
    /// the stock belongs.
    ORM_FIELD(industry, Field<"industry", std::string, orm::not_null_t>)

    /// The exchange on which the stock is listed, this indicates the
    /// marketplace where the stock is traded.
    ORM_FIELD(exchange, Field<"exchange", std::string, orm::not_null_t>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(
        StockRow,
        id,
        ticker,
        instrumentId,
        currency,
        shortName,
        longName,
        assetClass,
        sector,
        industry,
        exchange
    )
    /// @endcond
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__STOCK_ROW_HPP__