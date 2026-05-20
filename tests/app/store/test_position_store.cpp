#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "app/store/account/account_session.hpp"
#include "app/store/position_store.hpp"
#include "config/id_types.hpp"
#include "finance/position.hpp"
#include "mock_services.hpp"
#include "utils/timestamp.hpp"

namespace
{

    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class PositionStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockPositionService> _mockService;
        app::AccountSession                         _session;
        std::unique_ptr<app::PositionStore>         _store;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        PositionStoreTest()
            : _mockService{
                  std::make_shared<tests::MockPositionService>()
              },
              _store{
                  std::make_unique<app::PositionStore>(_mockService, _session)
              }
        {
        }
    };

}   // namespace

TEST_F(PositionStoreTest, CreatePosition_ReturnsValidLocalId)
{
    const finance::Position pos{Timestamp::fromInt64(TEST_TS)};

    const auto id = _store->createPosition(pos);

    EXPECT_TRUE(id.isValid());
}

TEST_F(PositionStoreTest, GetAllPositions_EmptyWhenNoAccounts)
{
    const auto positions = _store->getAllPositions();

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionStoreTest, GetOpenPositions_EmptyWhenNoAccounts)
{
    const auto positions = _store->getOpenPositions();

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionStoreTest, IsDirty_FalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(PositionStoreTest, IsDirty_TrueAfterCreatePosition)
{
    static_cast<void>(
        _store->createPosition(finance::Position{Timestamp::fromInt64(TEST_TS)})
    );

    EXPECT_TRUE(_store->isDirty());
}

TEST_F(PositionStoreTest, Commit_NewPosition_CallsService)
{
    static_cast<void>(
        _store->createPosition(finance::Position{Timestamp::fromInt64(TEST_TS)})
    );

    _store->commit();

    EXPECT_EQ(_mockService->createCallCount, 1);
}

TEST_F(PositionStoreTest, CreateMultiplePositions_IdsAreDistinct)
{
    const auto id1 =
        _store->createPosition(
            finance::Position{Timestamp::fromInt64(TEST_TS)}
        );
    const auto id2 =
        _store->createPosition(
            finance::Position{Timestamp::fromInt64(TEST_TS + 1)}
        );

    EXPECT_NE(id1, id2);
}
