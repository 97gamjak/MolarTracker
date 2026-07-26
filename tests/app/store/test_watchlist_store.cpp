#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "mock_services.hpp"
#include "store/watchlist_store.hpp"

namespace
{

    class WatchlistStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockWatchlistService> _mockService;
        std::unique_ptr<store::WatchlistStore>       _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        WatchlistStoreTest()
            : _mockService{std::make_shared<tests::MockWatchlistService>()},
              _store{std::make_unique<store::WatchlistStore>(_mockService)}
        {
        }
    };

}   // namespace

TEST_F(WatchlistStoreTest, GetAllWatchlistsEmptyInitially)
{
    EXPECT_TRUE(_store->getAllWatchlists().empty());
}

TEST_F(WatchlistStoreTest, CreateWatchlistStagesEntry)
{
    static_cast<void>(_store->createWatchlist("Tech Stocks"));

    const auto watchlists = _store->getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getName(), "Tech Stocks");
}

TEST_F(WatchlistStoreTest, CreateWatchlistDoesNotCallServiceImmediately)
{
    static_cast<void>(_store->createWatchlist("Tech Stocks"));

    EXPECT_EQ(_mockService->createCallCount, 0);
}

TEST_F(WatchlistStoreTest, CommitNewWatchlistCallsService)
{
    static_cast<void>(_store->createWatchlist("Tech Stocks"));

    _store->commit();

    EXPECT_EQ(_mockService->createCallCount, 1);
}

TEST_F(WatchlistStoreTest, IsDirtyFalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(WatchlistStoreTest, IsDirtyTrueAfterCreateWatchlist)
{
    static_cast<void>(_store->createWatchlist("Tech Stocks"));

    EXPECT_TRUE(_store->isDirty());
}

TEST_F(WatchlistStoreTest, IsDirtyFalseAfterCommit)
{
    static_cast<void>(_store->createWatchlist("Tech Stocks"));

    _store->commit();

    EXPECT_FALSE(_store->isDirty());
}

TEST_F(WatchlistStoreTest, GetWatchlistReturnsCreatedEntry)
{
    const auto id = _store->createWatchlist("Tech Stocks");

    const auto watchlist = _store->getWatchlist(id);

    ASSERT_TRUE(watchlist.has_value());
    EXPECT_EQ(watchlist->getName(), "Tech Stocks");
}

TEST_F(WatchlistStoreTest, GetWatchlistReturnsNulloptForUnknownId)
{
    const auto watchlist = _store->getWatchlist(WatchlistId{999});

    EXPECT_FALSE(watchlist.has_value());
}

TEST_F(WatchlistStoreTest, ReloadLoadsWatchlistsFromService)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId::invalid(), "Preloaded", Timestamp{});

    _store->reload();

    const auto watchlists = _store->getAllWatchlists();
    ASSERT_EQ(watchlists.size(), 1U);
    EXPECT_EQ(watchlists[0].getName(), "Preloaded");
}

TEST_F(WatchlistStoreTest, RenameWatchlistCallsServiceImmediately)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->renameWatchlist(WatchlistId{1}, "Renamed");

    EXPECT_EQ(_mockService->renameCallCount, 1);
}

TEST_F(WatchlistStoreTest, RenameWatchlistUpdatesCachedName)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->renameWatchlist(WatchlistId{1}, "Renamed");

    const auto watchlist = _store->getWatchlist(WatchlistId{1});
    ASSERT_TRUE(watchlist.has_value());
    EXPECT_EQ(watchlist->getName(), "Renamed");
}

TEST_F(WatchlistStoreTest, RenameWatchlistDoesNotMarkStoreDirty)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->renameWatchlist(WatchlistId{1}, "Renamed");

    EXPECT_FALSE(_store->isDirty());
}

TEST_F(WatchlistStoreTest, DeleteWatchlistCallsServiceImmediately)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->deleteWatchlist(WatchlistId{1});

    EXPECT_EQ(_mockService->deleteCallCount, 1);
}

TEST_F(WatchlistStoreTest, DeleteWatchlistRemovesFromCache)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->deleteWatchlist(WatchlistId{1});

    EXPECT_TRUE(_store->getAllWatchlists().empty());
}

TEST_F(WatchlistStoreTest, AddSymbolCallsServiceImmediately)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->addSymbol(WatchlistId{1}, "AAPL");

    EXPECT_EQ(_mockService->addSymbolCallCount, 1);
}

TEST_F(WatchlistStoreTest, AddSymbolUpdatesCachedSymbols)
{
    _mockService->preloadedWatchlists
        .emplace_back(WatchlistId{1}, "Tech Stocks", Timestamp{});
    _store->reload();

    _store->addSymbol(WatchlistId{1}, "AAPL");

    const auto watchlist = _store->getWatchlist(WatchlistId{1});
    ASSERT_TRUE(watchlist.has_value());
    ASSERT_EQ(watchlist->getSymbols().size(), 1U);
    EXPECT_EQ(watchlist->getSymbols()[0], "AAPL");
}

TEST_F(WatchlistStoreTest, RemoveSymbolCallsServiceImmediately)
{
    _mockService->preloadedWatchlists.emplace_back(
        WatchlistId{1},
        "Tech Stocks",
        Timestamp{},
        std::vector<std::string>{"AAPL"}
    );
    _store->reload();

    _store->removeSymbol(WatchlistId{1}, "AAPL");

    EXPECT_EQ(_mockService->removeSymbolCallCount, 1);
}

TEST_F(WatchlistStoreTest, RemoveSymbolUpdatesCachedSymbols)
{
    _mockService->preloadedWatchlists.emplace_back(
        WatchlistId{1},
        "Tech Stocks",
        Timestamp{},
        std::vector<std::string>{"AAPL"}
    );
    _store->reload();

    _store->removeSymbol(WatchlistId{1}, "AAPL");

    const auto watchlist = _store->getWatchlist(WatchlistId{1});
    ASSERT_TRUE(watchlist.has_value());
    EXPECT_TRUE(watchlist->getSymbols().empty());
}
