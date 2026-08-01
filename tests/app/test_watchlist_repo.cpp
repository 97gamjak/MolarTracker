#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "db/database.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/watchlist_repo.hpp"
#include "test_fixtures.hpp"

namespace
{

    class WatchlistRepoTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile   _tempFile;
        db::Database        _db;
        repo::WatchlistRepo _repo;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        WatchlistRepoTest() : _db{_tempFile.path()}, _repo{_db}
        {
            repo::MigrationRunner{_db};
        }
    };

}   // namespace

TEST_F(WatchlistRepoTest, CreateWatchlistReturnsValidId)
{
    finance::Watchlist watchlist{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    const auto id = _repo.createWatchlist(watchlist);

    EXPECT_GT(id.value(), 0);
}

TEST_F(WatchlistRepoTest, CreateWatchlistPersistsWithEmptySymbols)
{
    const finance::Watchlist watchlist{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    const auto id = _repo.createWatchlist(watchlist);

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getId(), id.value());
    EXPECT_EQ(watchlists[0].getName(), "Tech Stocks");
    EXPECT_TRUE(watchlists[0].getSymbols().empty());
}

TEST_F(WatchlistRepoTest, GetAllWatchlistsEmptyInitially)
{
    EXPECT_TRUE(_repo.getAllWatchlists().empty());
}

TEST_F(WatchlistRepoTest, GetAllWatchlistsReturnsAllCreated)
{
    const finance::Watchlist watchlist1{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    const finance::Watchlist watchlist2{
        WatchlistId::invalid(),
        "Dividend Plays",
        Timestamp{}
    };
    static_cast<void>(_repo.createWatchlist(watchlist1));
    static_cast<void>(_repo.createWatchlist(watchlist2));

    const auto watchlists = _repo.getAllWatchlists();

    EXPECT_EQ(watchlists.size(), 2U);
}

TEST_F(WatchlistRepoTest, DeleteWatchlistRemovesIt)
{
    finance::Watchlist watchlist{
        WatchlistId::invalid(),
        "Tech Stocks",
        Timestamp{}
    };
    const auto id = _repo.createWatchlist(watchlist);

    _repo.deleteWatchlist(id.value());

    EXPECT_TRUE(_repo.getAllWatchlists().empty());
}
