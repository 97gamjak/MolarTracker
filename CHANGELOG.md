# Changelog

All changes and updates, that are relevant for a user will be documented here

## Next Release

### Bug Fix

- Fix silent failure when creating a duplicate account — the operation now
  correctly raises an error instead of throwing an unexpected exception type
- Fix profile update silently doing nothing — updates now correctly modify the
  targeted row
- Fix profile deletion crashing with a SQL syntax error

### Tests

- Add unit tests for `AccountRepo` covering `createAccount` and `getAllAccounts`
- Add unit tests for `ProfileRepo` covering `create`, `get` (by ID and name),
  `getAll`, `update`, and `remove`

<!-- insertion marker -->
## [0.2.2](https://github.com/repo/owner/releases/tag/0.2.2) - 2026-05-10

### Bug Fix

- Fix: Adding valid tickers did not work anymore after last bug fix

## [0.2.1](https://github.com/repo/owner/releases/tag/0.2.1) - 2026-05-07

### Bug Fix

- Fix: if ticker is valid but has an unknown currency now an error will be displayed instead of aborting the application

## [0.2.0](https://github.com/repo/owner/releases/tag/0.2.0) - 2026-05-05

#### Finance

- Add possibility to add stocks via ticker and yfinance to database
- Add possibility to create stock transactions

## [0.1.0](https://github.com/repo/owner/releases/tag/0.1.0) - 2026-04-22

### Features

#### General Settings

- Add first prototype of settings dialog for a centralized place to set settings

## [0.0.5](https://github.com/repo/owner/releases/tag/0.0.5) - 2026-04-18

### Features

#### Transactions

- Create transactions is now really only possible with the max amount of decimal places
- Added some nice formatting for amount input in create transaction dialog
- Now it is possible to add securities to a database (with basic info from yahoo finance) and see an overview of them

### Bug Fixes

- Fixed: creating new profile without immediate save works now
- Fixed: `Create Transaction` does not get the last used transaction type for creating transactions
- Fixed: having same transaction multiple times in table view when saving after creating
- Fixed: make it possible to have decimal numbers for transaction amounts

## [0.0.4](https://github.com/repo/owner/releases/tag/0.0.4) - 2026-04-18

### Feature

#### Logging

- Change debug slots to hierarchical approach -> now also usable in debug slots selection dialog
- Now a symlink to the latest log file called `molar_tracker_latest.log` is created

#### UI

- Add Sidebar to main window

#### Finance

- Add possibility to create CashAccounts
- Adding first draft of Single Account Pages
- Add possibility to add first Transactions and see an overview (cash transactions only)

### BugFix

- Fix arrangement of text and buttons in log file viewer

## [0.0.3](https://github.com/repo/owner/releases/tag/0.0.3) - 2026-02-22

### Documentation

- Adding developer and API docs via doxygen

### Bug Fixes

- If no default profile is set or exists at startup the error cases are now fixed and everything should behave as expected
- Fix startup profile existence to remain inside dialogs if no user was added or selected
- Fix CI building issue for Linux releases 

### Features

#### Startup Profile Selection/Adding

- Added input validators for startup profile fields and improved the first `quick sign in` dialog with inline error messages and visual feedback for invalid inputs

#### UI

- The App title shows now if there are some unsaved changes or not

## [0.0.2](https://github.com/repo/owner/releases/tag/0.0.2) - 2026-02-07

## [0.0.1](https://github.com/repo/owner/releases/tag/0.0.1) - 2026-02-07

### New Features

#### UI

- Main window always opens in full screen mode
- Add Dialog to set Debug flags for logging (Debug Menu)
- Add Dialog to display current log file (Debug Menu)
- Display correct version and github url in `About` request

#### Error Handling

- Add Global Exception Dialog for unrecoverable errors
- Log errors show now file, line and function

### Bug Fixes

#### UI

- make main window resizable again
- make add profile startup dialog center above main window

#### Logging

- Fix formatting issue in log file










