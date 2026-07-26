#include <gtest/gtest.h>

#include <memory>
#include <string>

#include "db/database.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/watchlist_repo.hpp"
#include "service/watchlist_service.hpp"
#include "test_fixtures.hpp"

namespace
{

    class WatchlistServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile                          _tempFile;
        db::Database                               _db;
        std::shared_ptr<repo::WatchlistRepo>       _repo;
        std::shared_ptr<service::WatchlistService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        WatchlistServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<repo::WatchlistRepo>(_db)},
              _service{std::make_shared<service::WatchlistService>(_repo)}
        {
            repo::MigrationRunner{_db};
        }
    };

}   // namespace

TEST_F(WatchlistServiceTest, CreateWatchlistReturnsValidId)
{
    const auto id = _service->createWatchlist("Tech Stocks");

    EXPECT_GT(id.value(), 0);
}

TEST_F(WatchlistServiceTest, GetAllWatchlistsEmptyInitially)
{
    EXPECT_TRUE(_service->getAllWatchlists().empty());
}

TEST_F(WatchlistServiceTest, GetAllWatchlistsReturnsCreatedWatchlist)
{
    static_cast<void>(_service->createWatchlist("Tech Stocks"));

    const auto watchlists = _service->getAllWatchlists();

    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getName(), "Tech Stocks");
}

TEST_F(WatchlistServiceTest, RenameWatchlistUpdatesName)
{
    const auto id = _service->createWatchlist("Tech Stocks");

    _service->renameWatchlist(id, "Big Tech");

    EXPECT_EQ(_service->getAllWatchlists()[0].getName(), "Big Tech");
}

TEST_F(WatchlistServiceTest, DeleteWatchlistRemovesIt)
{
    const auto id = _service->createWatchlist("Tech Stocks");

    _service->deleteWatchlist(id);

    EXPECT_TRUE(_service->getAllWatchlists().empty());
}

TEST_F(WatchlistServiceTest, AddAndRemoveSymbolRoundTrips)
{
    const auto id = _service->createWatchlist("Tech Stocks");

    _service->addSymbol(id, "AAPL");
    EXPECT_EQ(_service->getAllWatchlists()[0].getSymbols().size(), 1U);

    _service->removeSymbol(id, "AAPL");
    EXPECT_TRUE(_service->getAllWatchlists()[0].getSymbols().empty());
}
