# Developer Changelog

All changes and updates, that are relevant for developers will be documented here

## Next Release

### Features

#### Profile Management

- Add UndoStack command for add profiles in a clean and persistent way

#### Utils

- Implement `Timestamp` static utility class (will be migrated to mstd in the future)
- Implement `RingFile` class for logging (will be migrated to mstd in the future)
- Implement smart config paths for Linux and Windows

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

#### CI

- make checks and updates for both `CHANGELOG.md` and `DEV-CHANGELOG.md` files available

#### Dependencies

- Add `mstd 0.0.2`

### Bugs

#### ORM

- Fix `index_seq` to return an index sequence with a correct length

#### Utils

- Update `Timestamp` to format local time using the current time zone instead of UTC

### Cleanup

- Make `AppConfig` decoupled from `app` and rename it to `Settings`

<!-- insertion marker -->