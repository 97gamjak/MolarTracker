#include <gtest/gtest.h>

#include <memory>

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
