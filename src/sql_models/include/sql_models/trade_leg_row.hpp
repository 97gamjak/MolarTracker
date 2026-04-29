#ifndef __SQL_MODELS__INCLUDE__SQL_MODELS__TRADE_LEG_ROW_HPP__
#define __SQL_MODELS__INCLUDE__SQL_MODELS__TRADE_LEG_ROW_HPP__

#include "config/id_types.hpp"
#include "orm/constraints.hpp"
#include "orm/field.hpp"
#include "orm/orm_model.hpp"
#include "sql_models/account_row.hpp"
#include "sql_models/instrument_row.hpp"
#include "sql_models/transaction_row.hpp"

struct TradeLegRow : public orm::ORMModel<"trade_leg">
{
    ORM_FIELD(id, IdField<TradeLegId>)

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

    ORM_FIELD(
        instrumentId,
        Field<
            "instrument_id",
            InstrumentId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                InstrumentRow,
                decltype(InstrumentRow::id)>,
            orm::not_null_t>
    )

    ORM_FIELD(quantity, Field<"quantity", int, orm::not_null_t>)

    ORM_FIELD(unitPrice, Field<"unit_price", micro_units, orm::not_null_t>)

    ORM_FIELD(currency, Field<"currency", Currency, orm::not_null_t>)

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
};

#endif   // __SQL_MODELS__INCLUDE__SQL_MODELS__TRADE_LEG_ROW_HPP__