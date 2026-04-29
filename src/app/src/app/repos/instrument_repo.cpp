#include "instrument_repo.hpp"

#include "app/factories/instrument_factory.hpp"
#include "config/id_types.hpp"
#include "sql_models/stock_row.hpp"

namespace app
{
    /**
     * @brief get a list of all stock tickers in the database
     *
     * @return std::vector<std::string>
     */
    std::vector<std::string> InstrumentRepo::getTickers()
    {
        auto results = _getCrud().get<StockRow>(_getDb());

        std::vector<std::string> tickers;
        tickers.reserve(results.size());

        for (const auto& row : results)
            tickers.push_back(row.ticker.value());

        return tickers;
    }

    void InstrumentRepo::addStock(const finance::Stock& stock)
    {
        auto [instrumentRow, stockRow] = InstrumentFactory::fromStock(stock);

        auto result = _getCrud().insert(_getDb(), instrumentRow);

        if (!result)
            throw std::runtime_error(
                "Failed to insert instrument row: " +
                result.error().getMessage()
            );

        stockRow.instrumentId = InstrumentId(result.value());
        result                = _getCrud().insert(_getDb(), stockRow);

        if (!result)
            throw std::runtime_error(
                "Failed to insert stock row: " + result.error().getMessage()
            );
    }

}   // namespace app