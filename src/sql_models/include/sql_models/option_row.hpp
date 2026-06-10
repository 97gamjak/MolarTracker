#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__OPTION_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__OPTION_ROW_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "config/quantity.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "sql_models/instrument_row.hpp"
#include "utils/timestamp.hpp"

/**
 * @brief Represents a row in the "option" database table, which contains
 * specific details about option instruments. This table has a one-to-one
 * relationship with the "instrument" table, where each row in the "option"
 * table corresponds to a row in the "instrument" table. The primary key of this
 * table is also a foreign key referencing the id field of the "instrument"
 * table, ensuring that each option instrument is associated with a valid
 * instrument entry. The option table includes fields specific to options, such
 * as the underlying instrument, option type (call or put), strike price,
 * expiration date, currency, and whether the option is a buy or sell. This
 * structured representation allows for detailed tracking and management of
 * option instruments within the financial application.
 *
 */
struct OptionRow : public orm::ORMModel<"option">
{
    /// as we have a 1:1 relationship between InstrumentRow and OptionRow, we
    /// disallow inserting an OptionRow without a corresponding InstrumentRow
    using insert_policy = orm::requires_paired_insert_t;

    /// The ID of the option instrument, this is the primary key for the option
    /// table.
    ORM_FIELD(id, IdField<OptionId>)

    /// The ID of the instrument associated with this option, this is a foreign
    /// key referencing the id field of the instrument table, and is used to
    /// associate this option instrument with a specific entry in the instrument
    /// table, allowing for a structured representation of different types of
    /// financial instruments while still maintaining a common base for all
    /// instruments.
    ORM_FIELD(
        instrumentId,
        InstrumentRow::
            template ForeignId<tableName, orm::CascadeDelete, "instrument_id">
    )

    /// The ID of the underlying instrument for this option, this is a foreign
    /// key referencing the id field of the instrument table, and is used to
    /// associate this option with its underlying asset, which is essential for
    /// understanding the option's value and behavior in relation to the
    /// underlying asset.
    ORM_FIELD(
        underlyingInstrumentId,
        InstrumentRow::template ForeignId<
            tableName,
            orm::RestrictDelete,   // we restrict deletion of the underlying
                                   // instrument to prevent orphaned options
            "underlying_instrument_id">
    )

    /// The type of the option, this indicates whether the option is a call or a
    /// put, which is a fundamental characteristic of the option that affects
    /// its payoff structure and trading strategy.
    ORM_FIELD(optionType, Field<"option_type", OptionType, orm::not_null_t>)

    /// The strike price of the option, this is the price at which the option
    /// can be exercised, and is a key factor in determining the option's value
    /// and potential profitability.
    ORM_FIELD(strikePrice, Field<"strike_price", micro_units, orm::not_null_t>)

    /// The expiration date of the option, this is the date on which the option
    /// expires and can no longer be exercised, and is a critical factor in
    /// determining the option's value and trading strategy, as options lose
    /// value as they approach their expiration date.
    ORM_FIELD(
        expirationDate,
        Field<"expiration_date", Timestamp, orm::not_null_t>
    )

    /// The currency of the option, this indicates the currency in which the
    /// option is traded, and is used for financial calculations and reporting,
    /// especially when dealing with options on underlying assets that may be
    /// denominated in different currencies, ensuring that the option is
    /// properly associated with the correct currency for financial calculations
    /// and reporting.
    ORM_FIELD(currency, Field<"currency", Currency, orm::not_null_t>)

    /// Indicates whether the option is a buy or sell, this is a field that
    /// specifies whether the option position is a long (buy) or short (sell)
    /// position, which is essential for understanding the option's risk profile
    /// and potential payoff, as well as for accurate financial reporting and
    /// analysis of the option's performance.
    ORM_FIELD(buySell, Field<"buy_sell", OptionBuySell, orm::not_null_t>)

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(
        OptionRow,
        id,
        instrumentId,
        underlyingInstrumentId,
        optionType,
        strikePrice,
        expirationDate,
        currency,
        buySell
    )
    /// @endcond

    /**
     * @brief Get the unique groups for the option table, this function defines
     * the unique constraints for the option table, ensuring that each option is
     * uniquely identified by the combination of its underlying instrument,
     * option type, strike price, and expiration date. This is important for
     * maintaining data integrity and preventing duplicate entries for the same
     * option, as well as for enabling efficient querying and management of
     * option data within the application.
     *
     * @return auto
     */
    static auto getUniqueGroups()
    {
        return orm::unique_set(
            orm::unique_group<
                &OptionRow::underlyingInstrumentId,
                &OptionRow::optionType,
                &OptionRow::strikePrice,
                &OptionRow::expirationDate>()
        );
    }
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__OPTION_ROW_HPP__