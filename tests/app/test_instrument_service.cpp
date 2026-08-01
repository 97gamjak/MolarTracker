#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "common/finance.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/instrument/stock.hpp"
#include "repo/instrument_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "service/instrument_service.hpp"
#include "test_fixtures.hpp"

namespace
{

    class InstrumentServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile                           _tempFile;
        db::Database                                _db;
        std::shared_ptr<repo::InstrumentRepo>       _repo;
        std::shared_ptr<service::InstrumentService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        InstrumentServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<repo::InstrumentRepo>(_db)},
              _service{std::make_shared<service::InstrumentService>(_repo)}
        {
            repo::MigrationRunner{_db};
        }

        [[nodiscard]] static finance::Stock makeStock(
            const std::string& ticker     = "AAPL",
            Currency           currency   = Currency::USD,
            AssetClass         assetClass = AssetClass::Stock
        )
        {
            return finance::Stock{
                ticker,
                currency,
                "Apple Inc.",
                "Apple Incorporated",
                "NASDAQ",
                "Consumer Electronics",
                "Technology",
                assetClass
            };
        }
    };

}   // namespace

TEST_F(InstrumentServiceTest, AddStockReturnsValidIds)
{
    const auto [stockId, instrumentId] =
        _service->addStock(makeStock()).value();

    EXPECT_GT(stockId.value(), 0);
    EXPECT_GT(instrumentId.value(), 0);
}

TEST_F(InstrumentServiceTest, GetTickersEmptyInitially)
{
    const auto tickers = _service->getTickers();

    EXPECT_TRUE(tickers.empty());
}

TEST_F(InstrumentServiceTest, GetTickersReturnsAddedTicker)
{
    static_cast<void>(_service->addStock(makeStock("AAPL")));

    const auto tickers = _service->getTickers();

    ASSERT_EQ(tickers.size(), 1U);
    EXPECT_EQ(tickers[0], "AAPL");
}

TEST_F(InstrumentServiceTest, StockExistsReturnsFalseForUnknown)
{
    EXPECT_FALSE(_service->stockExists("UNKNOWN"));
}

TEST_F(InstrumentServiceTest, StockExistsReturnsTrueForExisting)
{
    static_cast<void>(_service->addStock(makeStock("GOOG")));

    EXPECT_TRUE(_service->stockExists("GOOG"));
}

TEST_F(InstrumentServiceTest, GetStockReturnsNulloptForUnknown)
{
    const auto stock = _service->getStock("UNKNOWN");

    EXPECT_FALSE(stock.has_value());
}

TEST_F(InstrumentServiceTest, GetStockReturnsStockForExisting)
{
    static_cast<void>(_service->addStock(makeStock("MSFT")));

    const auto stock = _service->getStock("MSFT");

    ASSERT_TRUE(stock.has_value());
    EXPECT_EQ(stock->getTicker(), "MSFT");
}

TEST_F(InstrumentServiceTest, GetStocksReturnsAllForEmptyIdFilter)
{
    static_cast<void>(_service->addStock(makeStock("AAPL")));
    static_cast<void>(_service->addStock(makeStock("GOOG")));

    const auto stocks = _service->getStocks({});

    EXPECT_EQ(stocks.size(), 2U);
}
