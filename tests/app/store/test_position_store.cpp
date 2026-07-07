#include <gtest/gtest.h>

#include <cstdint>
#include <memory>

#include "finance/account/accounts.hpp"
#include "finance/position.hpp"
#include "mock_services.hpp"
#include "store/position_store.hpp"
#include "utils/timestamp.hpp"

namespace
{

    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class PositionStoreTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        std::shared_ptr<tests::MockPositionService> _mockService;
        std::unique_ptr<store::PositionStore>       _store;
        finance::AccountsView                       _accountSession;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        PositionStoreTest()
            : _mockService{std::make_shared<tests::MockPositionService>()},
              _store{std::make_unique<store::PositionStore>(_mockService)}
        {
        }
    };

}   // namespace

TEST_F(PositionStoreTest, CreatePositionReturnsValidLocalId)
{
    const finance::Position pos{
        PositionId::invalid(),
        AccountId{1},
        Timestamp::fromInt64(TEST_TS)
    };

    const auto id = _store->createPosition(pos);

    EXPECT_TRUE(id.isValid());
}

TEST_F(PositionStoreTest, GetAllPositionsEmptyWhenNoAccounts)
{
    const auto positions = _store->getAllPositions(_accountSession);

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionStoreTest, GetOpenPositionsEmptyWhenNoAccounts)
{
    const auto positions = _store->getOpenPositions(_accountSession.getIds());

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionStoreTest, IsDirtyFalseInitially)
{
    EXPECT_FALSE(_store->isDirty());
}

TEST_F(PositionStoreTest, IsDirtyTrueAfterCreatePosition)
{
    static_cast<void>(_store->createPosition(
        finance::Position{
            PositionId{-2},
            AccountId{1},
            Timestamp::fromInt64(TEST_TS)
        }
    ));

    EXPECT_TRUE(_store->isDirty());
}

TEST_F(PositionStoreTest, CommitNewPositionCallsService)
{
    static_cast<void>(_store->createPosition(
        finance::Position{
            PositionId{-2},
            AccountId{1},
            Timestamp::fromInt64(TEST_TS)
        }
    ));

    _store->commit();

    EXPECT_EQ(_mockService->createCallCount, 1);
}

TEST_F(PositionStoreTest, CreateMultiplePositionsIdsAreDistinct)
{
    const auto id1 = _store->createPosition(
        finance::Position{
            PositionId::invalid(),
            AccountId{1},
            Timestamp::fromInt64(TEST_TS)
        }
    );
    const auto id2 = _store->createPosition(
        finance::Position{
            PositionId::invalid(),
            AccountId{1},
            Timestamp::fromInt64(TEST_TS + 1)
        }
    );

    EXPECT_NE(id1, id2);
}
