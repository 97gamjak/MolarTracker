#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "common/timestamp.hpp"
#include "db/database.hpp"
#include "finance/position.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/position_repo.hpp"
#include "service/position_service.hpp"
#include "test_fixtures.hpp"

namespace
{

    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class PositionServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile                         _tempFile;
        db::Database                              _db;
        std::shared_ptr<repo::PositionRepo>       _repo;
        std::shared_ptr<service::PositionService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        PositionServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<repo::PositionRepo>(_db)},
              _service{std::make_shared<service::PositionService>(_repo)}
        {
            repo::MigrationRunner{_db};
        }
    };

}   // namespace

TEST_F(PositionServiceTest, CreatePositionReturnsValidId)
{
    const finance::Position position{Timestamp::fromInt64(TEST_TS)};

    const auto id = _service->createPosition(position);

    EXPECT_GT(id.value(), 0);
}

TEST_F(PositionServiceTest, GetAllPositionsEmptyForEmptyAccountSet)
{
    const auto positions = _service->getAllPositions({});

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionServiceTest, GetAllOpenPositionsEmptyForEmptyAccountSet)
{
    const auto positions = _service->getAllOpenPositions({});

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionServiceTest, CreateMultiplePositionsIdsAreDistinct)
{
    const finance::Position position1{Timestamp::fromInt64(TEST_TS)};
    const finance::Position position2{Timestamp::fromInt64(TEST_TS + 1)};

    const auto id1 = _service->createPosition(position1);
    const auto id2 = _service->createPosition(position2);

    EXPECT_NE(id1, id2);
}
