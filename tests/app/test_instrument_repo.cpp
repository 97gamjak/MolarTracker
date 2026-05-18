#include <gtest/gtest.h>

#include <filesystem>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "app/migration/migration_runner.hpp"
#include "app/repos/instrument_repo.hpp"
#include "config/finance.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/instrument/stock.hpp"

namespace
{

    std::filesystem::path unique_db_path()
    {
        namespace fs = std::filesystem;

        fs::path base;
        try
        {
            base = fs::temp_directory_path();
        }
        catch (...)
        {
            base = fs::current_path();
        }

        std::random_device random;
        const auto         r1 = static_cast<unsigned>(random());
        const auto         r2 = static_cast<unsigned>(random());

        return base /
               ("molartracker_instrument_repo_test_" + std::to_string(r1) +
                "_" + std::to_string(r2) + ".sqlite");
    }

    class TempDbFile
    {
       public:
        TempDbFile() : _path(unique_db_path()) {}

        TempDbFile(const TempDbFile&)            = delete;
        TempDbFile& operator=(const TempDbFile&) = delete;
        TempDbFile(TempDbFile&&)                 = default;
        TempDbFile& operator=(TempDbFile&&)      = default;

        ~TempDbFile()
        {
            std::error_code ec;
            std::filesystem::remove(_path, ec);
        }

        [[nodiscard]] const std::filesystem::path& path() const noexcept
        {
            return _path;
        }

       private:
        std::filesystem::path _path;
    };

    class InstrumentRepoTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        TempDbFile          _tempFile;
        db::Database        _db;
        app::InstrumentRepo _repo;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        InstrumentRepoTest() : _db{_tempFile.path()}, _repo{_db}
        {
            app::MigrationRunner{_db};
        }

        [[nodiscard]] static finance::Stock makeStock(
            const std::string& ticker    = "AAPL",
            Currency           currency  = Currency::USD,
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

TEST_F(InstrumentRepoTest, AddStock_ReturnsValidStockId)
{
    const auto [stockId, instrumentId] = _repo.addStock(makeStock());

    EXPECT_GT(stockId.value(), 0);
}

TEST_F(InstrumentRepoTest, AddStock_ReturnsValidInstrumentId)
{
    const auto [stockId, instrumentId] = _repo.addStock(makeStock());

    EXPECT_GT(instrumentId.value(), 0);
}

TEST_F(InstrumentRepoTest, AddStock_TwoStocks_IdsAreDistinct)
{
    const auto [stockId1, instrId1] = _repo.addStock(makeStock("AAPL"));
    const auto [stockId2, instrId2] = _repo.addStock(makeStock("GOOG"));

    EXPECT_NE(stockId1, stockId2);
    EXPECT_NE(instrId1, instrId2);
}

// ---------------------------------------------------------------------------
// addStock — duplicate ticker
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, AddStock_DuplicateTicker_Throws)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    EXPECT_THROW(
        static_cast<void>(_repo.addStock(makeStock("AAPL"))),
        std::runtime_error
    );
}

// ---------------------------------------------------------------------------
// stockExists
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, StockExists_NonExistentTicker_ReturnsFalse)
{
    const auto exists = _repo.stockExists("AAPL");

    EXPECT_FALSE(exists);
}

TEST_F(InstrumentRepoTest, StockExists_ExistingTicker_ReturnsTrue)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto exists = _repo.stockExists("AAPL");

    EXPECT_TRUE(exists);
}

TEST_F(InstrumentRepoTest, StockExists_DifferentTicker_ReturnsFalse)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto exists = _repo.stockExists("GOOG");

    EXPECT_FALSE(exists);
}

// ---------------------------------------------------------------------------
// getTickers
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, GetTickers_EmptyDatabase_ReturnsEmpty)
{
    const auto tickers = _repo.getTickers();

    EXPECT_TRUE(tickers.empty());
}

TEST_F(InstrumentRepoTest, GetTickers_AfterAddingStock_ContainsTicker)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto tickers = _repo.getTickers();

    ASSERT_EQ(tickers.size(), 1U);
    EXPECT_EQ(tickers[0], "AAPL");
}

TEST_F(InstrumentRepoTest, GetTickers_MultipleStocks_ReturnsAllTickers)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));
    static_cast<void>(_repo.addStock(makeStock("GOOG")));
    static_cast<void>(_repo.addStock(makeStock("MSFT")));

    const auto tickers = _repo.getTickers();

    ASSERT_EQ(tickers.size(), 3U);

    const std::set<std::string> tickerSet(tickers.begin(), tickers.end());
    EXPECT_TRUE(tickerSet.count("AAPL") > 0);
    EXPECT_TRUE(tickerSet.count("GOOG") > 0);
    EXPECT_TRUE(tickerSet.count("MSFT") > 0);
}

// ---------------------------------------------------------------------------
// getStock (by ticker)
// ---------------------------------------------------------------------------

TEST_F(InstrumentRepoTest, GetStock_NonExistentTicker_ReturnsNullopt)
{
    const auto result = _repo.getStock("AAPL");

    EXPECT_FALSE(result.has_value());
}

TEST_F(InstrumentRepoTest, GetStock_ExistingTicker_ReturnsStock)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));

    const auto result = _repo.getStock("AAPL");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getTicker(), "AAPL");
}

TEST_F(InstrumentRepoTest, GetStock_PreservesAllFields)
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

TEST_F(InstrumentRepoTest, GetStock_AssignedIdIsPositive)
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

TEST_F(InstrumentRepoTest, GetStocks_EmptyDatabase_ReturnsEmpty)
{
    const auto stocks = _repo.getStocks({});

    EXPECT_TRUE(stocks.empty());
}

TEST_F(InstrumentRepoTest, GetStocks_EmptyIdSet_ReturnsAllStocks)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));
    static_cast<void>(_repo.addStock(makeStock("GOOG")));

    const auto stocks = _repo.getStocks({});

    EXPECT_EQ(stocks.size(), 2U);
}

TEST_F(InstrumentRepoTest, GetStocks_SpecificId_ReturnsOnlyMatchingStock)
{
    static_cast<void>(_repo.addStock(makeStock("AAPL")));
    const auto [stockId, instrId] = _repo.addStock(makeStock("GOOG"));

    const idSet<InstrumentId> ids{instrId};
    const auto                stocks = _repo.getStocks(ids);

    ASSERT_EQ(stocks.size(), 1U);
    EXPECT_EQ(stocks[0].getTicker(), "GOOG");
}

TEST_F(InstrumentRepoTest, GetStocks_MultipleIds_ReturnsMatchingStocks)
{
    const auto [stockId1, instrId1] = _repo.addStock(makeStock("AAPL"));
    const auto [stockId2, instrId2] = _repo.addStock(makeStock("GOOG"));
    static_cast<void>(_repo.addStock(makeStock("MSFT")));

    const idSet<InstrumentId> ids{instrId1, instrId2};
    const auto                stocks = _repo.getStocks(ids);

    ASSERT_EQ(stocks.size(), 2U);

    std::set<std::string> tickers;
    for (const auto& s : stocks)
        tickers.insert(s.getTicker());

    EXPECT_TRUE(tickers.count("AAPL") > 0);
    EXPECT_TRUE(tickers.count("GOOG") > 0);
    EXPECT_FALSE(tickers.count("MSFT") > 0);
}
