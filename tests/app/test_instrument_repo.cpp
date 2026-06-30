#include <gtest/gtest.h>

#include <set>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/instrument/instrument_predicates.hpp"
#include "finance/instrument/stock.hpp"
#include "repo/instrument_repo.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/repo_errors.hpp"
#include "test_fixtures.hpp"
#include "utils/finance.hpp"

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
    const auto [stockId, instrumentId] = _repo.addStock(makeStock());

    EXPECT_GT(stockId.value(), 0);
}

TEST_F(InstrumentRepoTest, AddStockReturnsValidInstrumentId)
{
    const auto [stockId, instrumentId] = _repo.addStock(makeStock());

    EXPECT_GT(instrumentId.value(), 0);
}

TEST_F(InstrumentRepoTest, AddStockTwoStocksIdsAreDistinct)
{
    const auto [stockId1, instrId1] = _repo.addStock(makeStock("AAPL"));
    const auto [stockId2, instrId2] = _repo.addStock(makeStock("GOOG"));

    EXPECT_NE(stockId1, stockId2);
    EXPECT_NE(instrId1, instrId2);
}

// ---------------------------------------------------------------------------
// addStock — duplicate ticker
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, AddStockDuplicateTickerThrows)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    EXPECT_THROW(
        static_cast<void>(_repo.addStock(makeStock("AAPL"))),
        repo::RepositoryException
    );
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
    const auto [stockId, instrId] = _repo.addStock(makeStock("GOOG"));

    const IdSet<InstrumentId> ids{instrId};
    finance::StockFilter      filter;
    filter.instrumentIds.combine(ids);

    const auto stocks = _repo.getStocks(filter);

    ASSERT_EQ(stocks.size(), 1U);
    EXPECT_EQ(stocks.getValues()[0].getTicker(), "GOOG");
}

TEST_F(InstrumentRepoTest, GetStocksMultipleIdsReturnsMatchingStocks)
{
    const auto [stockId1, instrId1] = _repo.addStock(makeStock("AAPL"));
    const auto [stockId2, instrId2] = _repo.addStock(makeStock("GOOG"));
    static_cast<void>(_repo.addStock(makeStock("MSFT")));

    const IdSet<InstrumentId> ids{instrId1, instrId2};
    finance::StockFilter      filter;
    filter.instrumentIds.combine(ids);

    const auto stocks = _repo.getStocks(filter);

    ASSERT_EQ(stocks.size(), 2U);

    std::set<std::string> tickers;
    for (const auto& [id, stock] : stocks)
        tickers.insert(stock.getTicker());

    EXPECT_TRUE(tickers.contains("AAPL"));
    EXPECT_TRUE(tickers.contains("GOOG"));
    EXPECT_FALSE(tickers.contains("MSFT"));
}
