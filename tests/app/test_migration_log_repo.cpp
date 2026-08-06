#include <gtest/gtest.h>

#include "db/database.hpp"
#include "repo/migration/migration_runner.hpp"
#include "repo/migration_log_repo.hpp"
#include "test_fixtures.hpp"

namespace
{

    class MigrationLogRepoTest : public ::testing::Test
    {
       protected:
        // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
        tests::TempDbFile      _tempFile;
        db::Database           _db;
        repo::MigrationLogRepo _repo;
        // NOLINTEND(misc-non-private-member-variables-in-classes)

        MigrationLogRepoTest() : _db{_tempFile.path()}, _repo{_db}
        {
            // A fresh database migrates from 0 straight to
            // MigrationRunner::getTargetVersion() in one batch; only steps
            // from the version that introduced migration_log onward can be
            // recorded (the table doesn't exist yet for earlier steps).
            repo::MigrationRunner{_db};
        }
    };

}   // namespace

TEST_F(MigrationLogRepoTest, CurrentVersionMatchesTargetAfterFreshMigration)
{
    EXPECT_EQ(_repo.getTargetDbVersion(), _repo.getCurrentDbVersion());
}

TEST_F(MigrationLogRepoTest, GetTargetDbVersionMatchesMigrationRunner)
{
    EXPECT_EQ(
        static_cast<std::int64_t>(repo::MigrationRunner::getTargetVersion()),
        _repo.getTargetDbVersion()
    );
}

TEST_F(MigrationLogRepoTest, OnlyStepsFromTableIntroductionAreLogged)
{
    const auto entries = _repo.getAll();

    // A brand-new database only ever logs the step that introduced the
    // migration_log table itself (and any steps after it) — never the
    // earlier, pre-audit-log migrations.
    ASSERT_FALSE(entries.empty());
    for (const auto& entry : entries)
    {
        EXPECT_LT(entry.fromVersion, entry.toVersion);
        EXPECT_FALSE(entry.releaseVersion.empty());
        EXPECT_FALSE(entry.appliedAt.empty());
    }

    EXPECT_LT(
        static_cast<std::int64_t>(entries.size()),
        _repo.getTargetDbVersion()
    );
}

TEST_F(MigrationLogRepoTest, EntriesAreOrderedOldestFirst)
{
    const auto entries = _repo.getAll();
    ASSERT_FALSE(entries.empty());

    for (std::size_t i = 1; i < entries.size(); ++i)
        EXPECT_LT(entries[i - 1].toVersion, entries[i].toVersion);
}
