#include <gtest/gtest.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "app/migration/migration_runner.hpp"
#include "app/repos/position_repo.hpp"
#include "app/services/position_service.hpp"
#include "config/id_types.hpp"
#include "db/database.hpp"
#include "finance/position.hpp"
#include "test_fixtures.hpp"
#include "utils/timestamp.hpp"

namespace
{

    constexpr std::int64_t TEST_TS = 1'715'000'000'000LL;

    class PositionServiceTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile                     _tempFile;
        db::Database                          _db;
        std::shared_ptr<app::PositionRepo>    _repo;
        std::shared_ptr<app::PositionService> _service;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        PositionServiceTest()
            : _db{_tempFile.path()},
              _repo{std::make_shared<app::PositionRepo>(_db)},
              _service{std::make_shared<app::PositionService>(_repo)}
        {
            app::MigrationRunner{_db};
        }
    };

}   // namespace

TEST_F(PositionServiceTest, CreatePosition_ReturnsValidId)
{
    const finance::Position position{Timestamp::fromInt64(TEST_TS)};

    const auto id = _service->createPosition(position);

    EXPECT_GT(id.value(), 0);
}

TEST_F(PositionServiceTest, GetAllPositions_EmptyForEmptyAccountSet)
{
    const auto positions = _service->getAllPositions({});

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionServiceTest, GetAllOpenPositions_EmptyForEmptyAccountSet)
{
    const auto positions = _service->getAllOpenPositions({});

    EXPECT_TRUE(positions.empty());
}

TEST_F(PositionServiceTest, CreateMultiplePositions_IdsAreDistinct)
{
    const finance::Position p1{Timestamp::fromInt64(TEST_TS)};
    const finance::Position p2{Timestamp::fromInt64(TEST_TS + 1)};

    const auto id1 = _service->createPosition(p1);
    const auto id2 = _service->createPosition(p2);

    EXPECT_NE(id1, id2);
}
