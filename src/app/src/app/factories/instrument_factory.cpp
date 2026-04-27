#include "instrument_factory.hpp"

#include "finance/cash.hpp"
#include "finance/cash_transaction.hpp"
#include "finance/security_transaction.hpp"
#include "sql_models/instrument_row.hpp"

namespace app
{

    InstrumentRow InstrumentFactory::toRow(
        const finance::TransactionDetail& detail
    )
    {
        struct Visitor
        {
            InstrumentRow operator()(const finance::CashTransaction& detail
            ) const
            {
                InstrumentRow row;
                row.id       = InstrumentId::invalid();
                row.kind     = InstrumentKind::Cash;
                row.currency = detail.getCurrency();
                return row;
            }

            InstrumentRow operator()(const finance::StockTransaction& detail
            ) const
            {
                InstrumentRow row;
                row.id       = InstrumentId::invalid();
                row.kind     = InstrumentKind::Stock;
                row.currency = detail.getCurrency();
                return row;
            }
        };

        return std::visit(Visitor{}, detail);
    }

    finance::TransactionDetail InstrumentFactory::fromRow(
        const InstrumentRow& row,
        micro_units          amount
    )
    {
        switch (row.kind.value())
        {
            case InstrumentKind::Cash:
                return finance::CashTransaction{
                    finance::Cash{row.currency.value(), amount}
                };
            case InstrumentKind::Stock:
                throw std::runtime_error("Unknown instrument kind");
        }

        throw std::logic_error("Unknown instrument kind");
    }

}   // namespace app
