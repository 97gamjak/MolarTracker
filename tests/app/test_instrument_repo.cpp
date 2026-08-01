#include <gtest/gtest.h>

#include <optional>
#include <set>
#include <string>
#include <vector>

#include "common/finance.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/instrument/securities_filter.hpp"
#include "finance/instrument/stock.hpp"
#include "repo/instrument_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "test_fixtures.hpp"

namespace
{

    class InstrumentRepoTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile    _tempFile;
        db::Database         _db;
        repo::InstrumentRepo _repo;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        InstrumentRepoTest() : _db{_tempFile.path()}, _repo{_db}
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

// ---------------------------------------------------------------------------
// addStock — returned IDs
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, AddStockReturnsValidStockId)
{
    const auto [stockId, instrumentId] = _repo.addStock(makeStock()).value();

    EXPECT_GT(stockId.value(), 0);
}

TEST_F(InstrumentRepoTest, AddStockReturnsValidInstrumentId)
{
    const auto [stockId, instrumentId] = _repo.addStock(makeStock()).value();

    EXPECT_GT(instrumentId.value(), 0);
}

TEST_F(InstrumentRepoTest, AddStockTwoStocksIdsAreDistinct)
{
    const auto [stockId1, instrId1] = _repo.addStock(makeStock("AAPL")).value();
    const auto [stockId2, instrId2] = _repo.addStock(makeStock("GOOG")).value();

    EXPECT_NE(stockId1, stockId2);
    EXPECT_NE(instrId1, instrId2);
}

// ---------------------------------------------------------------------------
// addStock — duplicate ticker
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, AddStockDuplicateTickerReturnsFalse)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    EXPECT_FALSE(_repo.addStock(makeStock("AAPL")));
}

// ---------------------------------------------------------------------------
// stockExists
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, StockExistsNonExistentTickerReturnsFalse)
{
    const auto exists = _repo.stockExists("AAPL");

    EXPECT_FALSE(exists);
}

TEST_F(InstrumentRepoTest, StockExistsExistingTickerReturnsTrue)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto exists = _repo.stockExists("AAPL");

    EXPECT_TRUE(exists);
}

TEST_F(InstrumentRepoTest, StockExistsDifferentTickerReturnsFalse)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto exists = _repo.stockExists("GOOG");

    EXPECT_FALSE(exists);
}

// ---------------------------------------------------------------------------
// getTickers
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, GetTickersEmptyDatabaseReturnsEmpty)
{
    const auto tickers = _repo.getTickers();

    EXPECT_TRUE(tickers.empty());
}

TEST_F(InstrumentRepoTest, GetTickersAfterAddingStockContainsTicker)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto tickers = _repo.getTickers();

    ASSERT_EQ(tickers.size(), 1U);
    EXPECT_EQ(tickers[0], "AAPL");
}

TEST_F(InstrumentRepoTest, GetTickersMultipleStocksReturnsAllTickers)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));
    static_cast<void>(_repo.addStock(makeStock("GOOG")));
    static_cast<void>(_repo.addStock(makeStock("MSFT")));

    const auto tickers = _repo.getTickers();

    ASSERT_EQ(tickers.size(), 3U);

    const std::set<std::string> tickerSet(tickers.begin(), tickers.end());
    EXPECT_TRUE(tickerSet.contains("AAPL"));
    EXPECT_TRUE(tickerSet.contains("GOOG"));
    EXPECT_TRUE(tickerSet.contains("MSFT"));
}

// ---------------------------------------------------------------------------
// getStock (by ticker)
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, GetStockNonExistentTickerReturnsNullopt)
{
    const auto result = _repo.getStock("AAPL");

    EXPECT_FALSE(result.has_value());
}

TEST_F(InstrumentRepoTest, GetStockExistingTickerReturnsStock)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto result = _repo.getStock("AAPL");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTicker(), "AAPL");
}

TEST_F(InstrumentRepoTest, GetStockPreservesAllFields)
{
    const auto stock = finance::Stock{
        "TSLA",
        Currency::USD,
        "Tesla",
        "Tesla Inc.",
        "NASDAQ",
        "Auto Manufacturers",
        "Consumer Cyclical",
        AssetClass::Stock
    };

    static_cast<void>(_repo.addStock(stock));

    const auto result = _repo.getStock("TSLA");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTicker(), "TSLA");
    EXPECT_EQ(result->getCurrency(), Currency::USD);
    EXPECT_EQ(result->getShortName(), "Tesla");
    EXPECT_EQ(result->getLongName(), "Tesla Inc.");
    EXPECT_EQ(result->getExchange(), "NASDAQ");
    EXPECT_EQ(result->getIndustry(), "Auto Manufacturers");
    EXPECT_EQ(result->getSector(), "Consumer Cyclical");
    EXPECT_EQ(result->getAssetClass(), AssetClass::Stock);
}

TEST_F(InstrumentRepoTest, GetStockAssignedIdIsPositive)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto result = _repo.getStock("AAPL");

    ASSERT_TRUE(result.has_value());
    EXPECT_GT(result->getId().value(), 0);
    EXPECT_GT(result->getInstrumentId().value(), 0);
}

// ---------------------------------------------------------------------------
// getStocks (by instrument ID set)
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, GetStocksEmptyDatabaseReturnsEmpty)
{
    const auto stocks = _repo.getStocks({});

    EXPECT_TRUE(stocks.empty());
}

TEST_F(InstrumentRepoTest, GetStocksEmptyIdSetReturnsAllStocks)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));
    static_cast<void>(_repo.addStock(makeStock("GOOG")));

    const auto stocks = _repo.getStocks({});

    EXPECT_EQ(stocks.size(), 2U);
}

TEST_F(InstrumentRepoTest, GetStocksSpecificIdReturnsOnlyMatchingStock)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));
    const auto [stockId, instrId] = _repo.addStock(makeStock("GOOG")).value();

    const IdSet<InstrumentId> ids{instrId};
    finance::SecuritiesFilter filter;
    filter.instrumentIds = ids;
    const auto stocks    = _repo.getStocks(filter);

    ASSERT_EQ(stocks.size(), 1U);
    EXPECT_EQ(stocks[0].getTicker(), "GOOG");
}

TEST_F(InstrumentRepoTest, GetStocksMultipleIdsReturnsMatchingStocks)
{
    const auto [stockId1, instrId1] = _repo.addStock(makeStock("AAPL")).value();
    const auto [stockId2, instrId2] = _repo.addStock(makeStock("GOOG")).value();
    static_cast<void>(_repo.addStock(makeStock("MSFT")));

    const IdSet<InstrumentId> ids{instrId1, instrId2};
    finance::SecuritiesFilter filter;
    filter.instrumentIds = ids;
    const auto stocks    = _repo.getStocks(filter);

    ASSERT_EQ(stocks.size(), 2U);

    std::set<std::string> tickers;
    for (const auto& stock : stocks)
        tickers.insert(stock.getTicker());

    EXPECT_TRUE(tickers.contains("AAPL"));
    EXPECT_TRUE(tickers.contains("GOOG"));
    EXPECT_FALSE(tickers.contains("MSFT"));
}
