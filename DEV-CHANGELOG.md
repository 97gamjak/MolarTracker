# Developer Changelog

All changes and updates, that are relevant for developers will be documented here

## Next Release

### Bug Fix

#### ORM

- Fix `orm::Crud::insert` to catch `db::SqliteError` from
  `executeToCompletion()` and return `std::unexpected(CrudError{...})`
  instead of propagating the exception, honouring the method's own
  `std::expected` return-type contract
- Fix `orm::Crud::update` binding the WHERE-clause parameters at index 0
  instead of after the SET-clause parameters, causing the primary-key
  predicate to always evaluate to NULL and update zero rows
- Fix `orm::Crud::update` propagating `db::SqliteError` (e.g. from a UNIQUE
  constraint violation) instead of returning
  `std::unexpected(CrudError{UpdateFailed, …})` as its return-type contract
  requires; add `CrudErrorType::UpdateFailed` to support this
- Fix `orm::Crud::deleteByPk` emitting `DELETE FROM <t> WHERE WHERE …`
  (double `WHERE`) by removing the manually appended `" WHERE "` that
  duplicated the keyword already produced by `getDBOperations()`
- Fix `orm::Crud::deleteByPk`: SQL contained duplicate `WHERE` keyword because `getDBOperations()` already prepends `WHERE` to the clause
- Fix `orm::Crud::update`: WHERE-clause parameters were bound at index 1 (overwriting the first SET parameter) instead of after all SET parameters
- Fix `orm::Query::getDBOperations`: `LIMIT` was silently ignored when no `ORDER BY` field was set due to an early return in the method
- Fix `orm::Crud::createTable()` (no-arg overload): `fixed_string` tableName was passed where `std::string_view` was expected without an explicit conversion
- Fix `orm::Query::orderBy()`: `fixed_string` field name was passed to `std::vector<pair<string,bool>>::push_back` without an explicit conversion

#### Finance

- Fix handling error if creating cash transaction fails
- Fix silent continuing for stock transaction creation if position exists but has not suitable instrument
- Improve error handling when creating a new position during stock transaction creation
- Fix retrieving only positions related to accounts that are loaded for the current profile

### UI

- Fix: make `positionAt` in `PositionSelectionTableModel` more robust by returning `std::optional` with boundary checks

### Tests

- Add `tests/app/test_account_repo.cpp` with GoogleTest fixture covering
  `AccountRepo::createAccount` (returns valid ID, persists data, enforces
  unique constraint, allows differing kind/profile) and
  `AccountRepo::getAllAccounts` (empty result, full set, profile isolation,
  correct domain data mapping)
- Add `tests/app/test_profile_repo.cpp` with GoogleTest fixture covering
  `ProfileRepo::create` (valid ID, duplicate name throws, with/without email),
  `ProfileRepo::get` by ID and name (hit and miss), `ProfileRepo::getAll`
  (empty, full set, correct data), `ProfileRepo::update` (name/email change,
  clear email, non-existent ID throws, duplicate name throws), and
  `ProfileRepo::remove` (deletes target, preserves others)

### Features

#### Finance

- Add position store, service and repo
- Add position creation when creating transactions

#### UI

- Add `ui/include/ui/include/utils/error.hpp` and `ui/src/ui/include/utils/error.cpp` for a generalized approach to display error messages

#### ORM

- Introduce `.in` for queries to make it easier to create where clauses for ranges

### Cleanup

- Speedup some compilation headers

<!-- insertion marker -->
### Testing

- Add 48 GoogleTest unit tests for `orm::Crud` covering `createTable`, `insert`, `batchInsert`, `get`, `getUnique`, `update`, `deleteByPk`, `addColumn`, `dropColumn`, `getJoined`, WHERE/ORDER BY/LIMIT query options, FK constraints (CASCADE and RESTRICT), unique constraints, and SQL execution tracking

## [0.2.3](https://github.com/repo/owner/releases/tag/0.2.3) - 2026-05-17

## [0.2.2](https://github.com/repo/owner/releases/tag/0.2.2) - 2026-05-10

## [0.2.1](https://github.com/repo/owner/releases/tag/0.2.1) - 2026-05-07

### Error Handling

- Introduce a common error type `Error` which can be used for `std::expected` approaches

## [0.2.0](https://github.com/repo/owner/releases/tag/0.2.0) - 2026-05-05

### Features

#### Finance

- Add security account type
- Add transaction type stock
- Add create stock dialog draft
- Add instrument repo/service and stockRow to db
- Add comment and timestamp fields to deposit and withdrawal widgets
- Add stock store and different methods for instrument repo / service
- Add yf client to retrieve basic ticker info
- Add security overview page
- Add option to add a security via `Add Security` dialog

#### HTTP

- Add general http client/response/request interface

#### Migration

- Show database path if an exception during migration occurs

#### Signal System

- Rework signal system of stores

## [0.1.0](https://github.com/repo/owner/releases/tag/0.1.0) - 2026-04-22

## [0.0.5](https://github.com/repo/owner/releases/tag/0.0.5) - 2026-04-18

## [0.0.4](https://github.com/repo/owner/releases/tag/0.0.4) - 2026-04-18

### Features

#### Settings

- Add first implementation approach for `ParamGroup` via `NumericVecParam`

#### Finance

- Add `Currency` enum and `CurrencyTraits` for handling different current
- Add `Cash` class for representing a cash amount together with a currency
- Add `CashAccount` class
- Add `Transaction` class
- Add `TransactionEntry` class
- Add `AccountRepo` with first `createCashAccount` method
- Add `AccountService` with first `createCashAccount` method
- Add first draft of `AccountStore`
- Include proper handling of `ProfileId` on `Account` database side
- Make `CashAccount` to general `Account` with `std::variant` details
- Add `TransactionRepo` and `TransactionService` with related factory methods
- Automatically add external accounts for cash accounts
- Add profileId as a uniqueness constraint for account names as two different profiles should be allowed to have the same name.
- Include `Transaction` category as side bar element
- Implement `getTransactions` in repo and a lot of preparing for future orm obstacles

#### Profile

- Add Listener to active `ProfileId`
- Introduce `ActiveProfile` type with Observable base in profile store to globally handle changes in profile

#### ORM

- Introduce `unique_group` to make it possible to have multiple fields as a single unique group
- Introduce bit-wise `FieldConstraints`
- Add `foreign_key_t` constraint
- Add `foreign_key_t` parsing for `ddl` member function of `Field`
- Implement `insert_policy` with corresponding `requires_paired_insert_t` type for 1:1 relationships
- Introduce `Joins` and `WhereClauses`

#### Store

- Introduce general base class `BaseStore` and interface `IStore`

#### Database

- Implement database version and corresponding migration backbone
- Implement migration for Account to have unique constraints for name + kind
- Add transaction, transaction_entry and instrument tables to migration

### Bugfix

- Make GNU compiler flags to be actually applied

### Testing

- Add test cases for `binder` struct
- Add test cases for `utils`

### Dependencies

- Upgrade `mstd` to `0.1.0`

### Documentation

- Add Claude Code instructions first version

### Cleanup

- Cleanup entire code base to be able to use `clangd-tidy` (not to be confused with `clang-tidy`)
- Cleanup NOLINT for new qt parent/child system
- Restructure project structure to reduce compile time dependencies

## [0.0.3](https://github.com/repo/owner/releases/tag/0.0.3) - 2026-02-22

### Bug Fixes

- Fix CI check for changes in CHANGELOG files

### Features

#### Commands

- Introduce `Commands` class as a batch container for better serialization of sub-commands

#### Utils

- implement `SemVer` class for versioning

#### Controllers

- implement container like `Controllers` type to separate controller handling from main window

#### Settings

- Add `version` and `oldVersion` to `Settings`
- Add `UISettings` including `LogViewerSettings`
- Add `reloadIntervalSec` and `autoReload` to `LogViewerSettings`
- Add generalized `Param...` handling for settings
- Add `lineWrap` settings for log viewer dialog
- Add variable to check if reboot is required to `ParamCore`
- Add settings for ring file logging
- Add subscription for default log level settings
- Add runtime check if a subscription is for a `ParamCore` that needs restart -> throw `ParamException`

#### Connections

- Add first implementation for connecting subscribers to custom classes
- Generalize subscription model by introducing `Observable` and `Signal` classes

#### Factories

- Introduce `ProfileFactory` for converting between Domain and Row

### CI

- Add doxygen checks
- Add cpp-checks and devops cpp checks
- Add first ctest via googletest

### Testing

- Adding unit tests for `Database` class
- Adding unit tests for `Statement` class
- Adding unit tests for `Transaction` class

## [0.0.2](https://github.com/repo/owner/releases/tag/0.0.2) - 2026-02-07

## [0.0.1](https://github.com/repo/owner/releases/tag/0.0.1) - 2026-02-07

### Features

#### Profile Management

- Add UndoStack command for add profiles in a clean and persistent way
- Add error handling for add profile commands

#### Utils

- Implement `Timestamp` static utility class (will be migrated to mstd in the future)
- Implement `RingFile` class for logging (will be migrated to mstd in the future)
- Implement smart config paths for Linux and Windows

#### Commands

- Add `ICommandError` for better handling error cases in commands

#### Config

- Implement global singleton for `Constants` to handle dynamic config and data dirs

#### Docs

- Add `CHANGELOG.md` and `DEV-CHANGELOG.md` support

#### UI

- Create `MolarTrackerApplication` wrapper for a cleaner setup
- Add `OK` and `Cancel` button to profile selection dialog (logic still missing)

#### Logging

- Use `std::cerr` as output if no RingFile is configured inside the logger
- Add `initGlobalConfig` for all singleton instances
- Add `UpdateDebugFlagsCommand` for handling undo and redo

### CI

- make checks and updates for both `CHANGELOG.md` and `DEV-CHANGELOG.md` files available
- introduce nice versioning scheme for artifacts on GitHub

### Dependencies

- Add `mstd 0.0.2`

### Bugs

#### ORM

- Fix `index_seq` to return an index sequence with a correct length

#### Utils

- Update `Timestamp` to format local time using the current time zone instead of UTC

### Cleanup

- Make `AppConfig` decoupled from `app` and rename it to `Settings`











