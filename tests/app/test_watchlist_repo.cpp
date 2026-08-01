#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "db/database.hpp"
#include "orm/crud.hpp"
#include "orm/query_options.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/watchlist_repo.hpp"
#include "sql_models/watchlist_instrument_row.hpp"
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
    const auto id = _repo.createWatchlist("Tech Stocks");

    EXPECT_GT(id.value(), 0);
}

TEST_F(WatchlistRepoTest, CreateWatchlistPersistsWithEmptySymbols)
{
    const auto id = _repo.createWatchlist("Tech Stocks");

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getId(), id);
    EXPECT_EQ(watchlists[0].getName(), "Tech Stocks");
    EXPECT_TRUE(watchlists[0].getSymbols().empty());
}

TEST_F(WatchlistRepoTest, GetAllWatchlistsEmptyInitially)
{
    EXPECT_TRUE(_repo.getAllWatchlists().empty());
}

TEST_F(WatchlistRepoTest, GetAllWatchlistsReturnsAllCreated)
{
    static_cast<void>(_repo.createWatchlist("Tech Stocks"));
    static_cast<void>(_repo.createWatchlist("Dividend Plays"));

    const auto watchlists = _repo.getAllWatchlists();

    EXPECT_EQ(watchlists.size(), 2U);
}

TEST_F(WatchlistRepoTest, RenameWatchlistUpdatesName)
{
    const auto id = _repo.createWatchlist("Tech Stocks");

    _repo.renameWatchlist(id.value(), "Big Tech");

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getName(), "Big Tech");
}

TEST_F(WatchlistRepoTest, RenameWatchlistPreservesCreatedAt)
{
    const auto id                = _repo.createWatchlist("Tech Stocks");
    const auto originalCreatedAt = _repo.getAllWatchlists()[0].getCreatedAt();

    _repo.renameWatchlist(id.value(), "Big Tech");

    const auto createdAtAfterRename =
        _repo.getAllWatchlists()[0].getCreatedAt();
    EXPECT_EQ(createdAtAfterRename.toInt64(), originalCreatedAt.toInt64());
}

TEST_F(WatchlistRepoTest, RenameNonExistentWatchlistThrows)
{
    EXPECT_THROW(
        _repo.renameWatchlist(WatchlistId{999}, "Ghost"),
        std::exception
    );
}

TEST_F(WatchlistRepoTest, DeleteWatchlistRemovesIt)
{
    const auto id = _repo.createWatchlist("Tech Stocks");

    _repo.deleteWatchlist(id.value());

    EXPECT_TRUE(_repo.getAllWatchlists().empty());
}

TEST_F(WatchlistRepoTest, AddSymbolPersistsSymbol)
{
    const auto id = _repo.createWatchlist("Tech Stocks");

    _repo.addSymbol(id.value(), "AAPL");

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    ASSERT_EQ(watchlists[0].getSymbols().size(), 1U);
    EXPECT_EQ(watchlists[0].getSymbols()[0], "AAPL");
}

TEST_F(WatchlistRepoTest, AddMultipleSymbolsPersistsAll)
{
    const auto id = _repo.createWatchlist("Tech Stocks");

    _repo.addSymbol(id.value(), "AAPL");
    _repo.addSymbol(id.value(), "MSFT");

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getSymbols().size(), 2U);
}

TEST_F(WatchlistRepoTest, AddDuplicateSymbolThrows)
{
    const auto id = _repo.createWatchlist("Tech Stocks");
    _repo.addSymbol(id.value(), "AAPL");

    EXPECT_NO_THROW(_repo.addSymbol(id.value(), "AAPL"));
}

TEST_F(WatchlistRepoTest, SameSymbolInDifferentWatchlistsSucceeds)
{
    const auto id1 = _repo.createWatchlist("Tech Stocks");
    const auto id2 = _repo.createWatchlist("Dividend Plays");

    _repo.addSymbol(id1.value(), "AAPL");

    EXPECT_NO_THROW(_repo.addSymbol(id2.value(), "AAPL"));
}

TEST_F(WatchlistRepoTest, RemoveSymbolDeletesIt)
{
    const auto id = _repo.createWatchlist("Tech Stocks");
    _repo.addSymbol(id.value(), "AAPL");
    _repo.addSymbol(id.value(), "MSFT");

    _repo.removeSymbol(id.value(), "AAPL");

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    ASSERT_EQ(watchlists[0].getSymbols().size(), 1U);
    EXPECT_EQ(watchlists[0].getSymbols()[0], "MSFT");
}

TEST_F(WatchlistRepoTest, RemoveSymbolNotInWatchlistIsNoOp)
{
    const auto id = _repo.createWatchlist("Tech Stocks");
    _repo.addSymbol(id.value(), "AAPL");

    EXPECT_NO_THROW(_repo.removeSymbol(id.value(), "GOOG"));

    const auto watchlists = _repo.getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getSymbols().size(), 1U);
}

TEST_F(WatchlistRepoTest, DeleteWatchlistCascadesToInstruments)
{
    const auto id = _repo.createWatchlist("Tech Stocks");
    _repo.addSymbol(id.value(), "AAPL");
    _repo.addSymbol(id.value(), "MSFT");

    _repo.deleteWatchlist(id.value());

    const auto query =
        orm::Query{}.where(WatchlistInstrumentRow::hasWatchlistId(id.value()));
    const auto remainingRows =
        orm::Crud().get<WatchlistInstrumentRow>(_db, query);

    EXPECT_TRUE(remainingRows.empty());
}
