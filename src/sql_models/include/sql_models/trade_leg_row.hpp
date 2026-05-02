#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRADE_LEG_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRADE_LEG_ROW_HPP__

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "sql_models/account_row.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/position_row.hpp"
#include "sql_models/transaction_row.hpp"

/**
 * @brief ORM model for the trade_leg table, representing a single leg of a
 * trade
 *
 */
struct TradeLegRow : public orm::ORMModel<"trade_leg">
{
    /// The ID of the trade leg, this is the primary key for the trade_leg table
    ORM_FIELD(id, IdField<TradeLegId>)

    /// The ID of the transaction this trade leg belongs to, this is a foreign
    /// key referencing the id field of the transaction table, and is used to
    /// associate this trade leg with a specific transaction.
    ORM_FIELD(
        transactionId,
        Field<
            "transaction_id",
            TransactionId,
            orm::foreign_key_t<
                orm::CascadeDelete,
                TransactionRow,
                decltype(TransactionRow::id)>,
            orm::not_null_t>
    )

    /// The ID of the account associated with this trade leg, this is a foreign
    /// key referencing the id field of the account table, and is used to
    /// associate this trade leg with a specific account.
    ORM_FIELD(
        accountId,
        Field<
            "account_id",
            AccountId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                AccountRow,
                decltype(AccountRow::id)>,
            orm::not_null_t>
    )

    /// The ID of the instrument being traded in this leg, this is a foreign key
    /// referencing the id field of the instrument table, and is used to
    /// associate this trade leg with a specific financial instrument.
    ORM_FIELD(
        instrumentId,
        InstrumentRow::template ForeignId<orm::RestrictDelete>
    )

    /// The quantity of the instrument being traded in this leg, this is a
    /// numeric field representing the amount of the instrument being bought or
    /// sold in this trade leg.
    ORM_FIELD(quantity, Field<"quantity", micro_units, orm::not_null_t>)

    /// The unit price of the instrument being traded in this leg, this is a
    /// numeric field representing the price per unit of the instrument being
    /// bought or sold in this trade leg.
    ORM_FIELD(unitPrice, Field<"unit_price", micro_units, orm::not_null_t>)

    /// The currency of the trade leg, this is a string field representing the
    /// currency in which the trade leg is denominated, and is used to specify
    /// the currency for the quantity and unit price fields, ensuring that the
    /// trade leg is properly associated with the correct currency for financial
    /// calculations and reporting.
    ORM_FIELD(currency, Field<"currency", Currency, orm::not_null_t>)

    ORM_FIELD(
        positionId,
        Field<
            "position_id",
            PositionId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                PositionRow,
                decltype(PositionRow::id)>>
    )

    /// @cond DOXYGEN_IGNORE
    ORM_FIELDS(
        TradeLegRow,
        id,
        transactionId,
        accountId,
        instrumentId,
        quantity,
        unitPrice,
        currency
    )
    /// @endcond
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRADE_LEG_ROW_HPP__