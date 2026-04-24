#include "instrument_repo.hpp"

#include <expected>

#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "db/transaction.hpp"
#include "orm/crud/crud_error.hpp"
#include "orm/query_options.hpp"

namespace app
{

    InstrumentRepo::InstrumentRepo(db::Database& db) : BaseRepo(db)
    {
        db::Transaction transaction{_getDb()};

        // here we create default cash instruments for all currencies
        for (const auto& currency : CurrencyMeta::values_view())
        {
            const auto instrument =
                InstrumentRow::createCashInstrument(currency);

            const auto result = _createInstrumentIfNotExists(instrument);

            if (!result)
            {
                // if we end up in here we really have something wrong going on
                throw std::runtime_error(
                    "Failed to create default cash instrument for currency: " +
                    CurrencyMeta::toString(currency)
                );
            }
        }

        transaction.commit();
    }

    /**
     * @brief get an instrument from the database
     *
     * @param row
     * @return std::optional<InstrumentId>
     */
    std::optional<InstrumentId> InstrumentRepo::getInstrumentId(
        const InstrumentRow& row
    )
    {
        switch (row.kind.value())
        {
            case InstrumentKind::Cash:
            {
                const auto query = orm::Query{}.where(
                    InstrumentRow::hasKind(InstrumentKind::Cash) &&
                    InstrumentRow::hasCurrency(row.currency.value())
                );

                const auto instrument =
                    _getCrud().getUnique<InstrumentRow>(_getDb(), query);

                if (instrument.has_value())
                    return instrument->id.value();

                return std::nullopt;
            }
            case InstrumentKind::Stock:
                throw std::runtime_error(
                    "Stock instruments are not supported yet"
                );
        }

        throw std::logic_error("Unknown instrument kind");
    }

    std::expected<InstrumentId, orm::CrudError> InstrumentRepo::
        _createInstrument(const InstrumentRow& row)
    {
        const auto result = _getCrud().insert(_getDb(), row);

        if (result)
            return row.id.value();

        return std::unexpected(result.error());
    }

    std::expected<InstrumentId, orm::CrudError> InstrumentRepo::
        _createInstrumentIfNotExists(const InstrumentRow& row)
    {
        const auto existingId = getInstrumentId(row);

        if (existingId)
            return existingId.value();

        return _createInstrument(row);
    }

}   // namespace app