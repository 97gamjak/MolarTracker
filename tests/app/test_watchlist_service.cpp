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
    const finance::Watchlist watchlist{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    const auto id = _service->createWatchlist(watchlist);

    EXPECT_GT(id.value(), 0);
}

TEST_F(WatchlistServiceTest, GetAllWatchlistsEmptyInitially)
{
    EXPECT_TRUE(_service->getAllWatchlists().empty());
}

TEST_F(WatchlistServiceTest, GetAllWatchlistsReturnsCreatedWatchlist)
{
    finance::Watchlist watchlist{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    static_cast<void>(_service->createWatchlist(watchlist));

    const auto watchlists = _service->getAllWatchlists();

    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getName(), "Tech Stocks");
}

TEST_F(WatchlistServiceTest, DeleteWatchlistRemovesIt)
{
    const finance::Watchlist watchlist{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    const auto id = _service->createWatchlist(watchlist);

    _service->deleteWatchlist(id.value());

    EXPECT_TRUE(_service->getAllWatchlists().empty());
}
