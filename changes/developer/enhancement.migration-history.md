- Add a `migration_log` table (`MigrationLogRow`, `IMigrationLogRepo`/`MigrationLogRepo`,
  `IMigrationLogService`/`MigrationLogService`) recording every applied
  migration step (from/to version, release, applied-at timestamp)
- `Migrations::migrate()` (`src/repo/migration/migration.cpp`) now logs each
  step automatically once the migration_log table exists (introduced in
  migration v17, bumping `MigrationRunner::DB_VERSION` 16 → 17) — future
  migrations get audit-logged with no extra code needed; earlier migrations
  (v1–v16) cannot be logged retroactively since the table didn't exist yet
- Add `ui::MigrationHistoryDialog` (Help → Migration History), wired through
  `HelpMenu`/`HelpMenuController`; `StoreContainer::getMigrationLogService()`
  passthrough added since this read-only diagnostic data doesn't need a
  Store (no dirty-tracking)
- Add `tests/app/test_migration_log_repo.cpp` covering the repo against a
  real SQLite migration run
