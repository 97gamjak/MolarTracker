#ifndef __SQL_MODELS__INSTRUMENT_ROW_HPP__
#define __SQL_MODELS__INSTRUMENT_ROW_HPP__

#include "config/id_types.hpp"
#include "finance/currency.hpp"
#include "orm/field.hpp"

struct InstrumentRow
{
    static constexpr std::string table_name = "instrument";

    orm::IdField<InstrumentId> id;

    orm::Field<"kind", InstrumentKind, orm::not_null_t> kind;
};

struct CashInstrumentRow
{
    static constexpr std::string table_name = "cash_instrument";

    orm::IdField<InstrumentId> id;

    orm::Field<"currency", finance::Currency, orm::not_null_t, orm::unique_t>
        currency;
};

#endif   // __SQL_MODELS__INSTRUMENT_ROW_HPP__