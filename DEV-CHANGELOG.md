# Developer Changelog

All changes and updates, that are relevant for developers will be documented here

## Next Release

### Features

#### Finance

- Add `Currency` enum and `CurrencyTraits` for handling different current

<!-- insertion marker -->
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

#### CI

- Add doxygen checks
- Add cpp-checks and devops cpp checks
- Add first ctest via googletest

#### Testing

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

#### CI

- make checks and updates for both `CHANGELOG.md` and `DEV-CHANGELOG.md` files available
- introduce nice versioning scheme for artifacts on GitHub

#### Dependencies

- Add `mstd 0.0.2`

### Bugs

#### ORM

- Fix `index_seq` to return an index sequence with a correct length

#### Utils

- Update `Timestamp` to format local time using the current time zone instead of UTC

### Cleanup

- Make `AppConfig` decoupled from `app` and rename it to `Settings`




