# Changelog

All changes and updates, that are relevant for a user will be documented here

## Next Release

### Bug Fix

- Fix silent failure when creating a duplicate account — the operation now
  correctly raises an error instead of throwing an unexpected exception type
- Fix profile update silently doing nothing — updates now correctly modify the
  targeted row
- Fix profile deletion crashing with a SQL syntax error
- Fix three ORM bugs exposed by unit tests: duplicate `WHERE` in `deleteByPk` SQL, wrong bind-parameter index in `update` WHERE clause, and `LIMIT` being silently ignored when no `ORDER BY` was set
- Fix default log level of settings is now also applied at startup
- Fix numeric settings (e.g. max log files, log age, window sizes) not being
  saved in the Settings dialog — only toggle/checkbox settings persisted
  before this fix

### Features

- Show a symbol before each account name in the sidebar: `●` for cash
  accounts and `▲` for security accounts
- Add a "Report Bug" button to the fatal error dialog; it pre-fills a GitHub
  issue with the exception details and app/OS info for you to review and
  submit
- Add a "Reset to Defaults" button to the Settings dialog to restore all
  settings back to their default values
- Automatically check for new MolarTracker releases on startup and every
  24 h; if a newer version is available, a dialog is shown with a link to
  the releases page and a "don't show again for this version" option
- Log files older than the configured maximum age (default: 30 days) are
  automatically removed at startup, preventing unbounded accumulation of old
  session log files. The limit is configurable via the new "Max Log Age (Days)"
  setting (0 disables age-based cleanup).
- Closing the main window while there are unsaved changes now shows a
  "Discard changes?" confirmation dialog; the window only closes if the
  user confirms
- Automatic database backup on every startup with a tiered rolling-window
  retention strategy: 5 most-recent backups, 1 per calendar week for 4 weeks,
  then 1 per calendar month indefinitely
- "Restore from Backup…" action in the Settings menu lets you select a backup,
  preview its timestamp, and restore in-place without restarting
- Add possibility to add fees when creating new stock or cash (deposit/withdrawal) transactions
- Add possibility to persist different log levels for different categories
- Show full exception stack trace when an exception occurs
- Add Help page accessible from Help → Help menu item, with PDF export
- show open positions in security accounts overview pages
- Make it possible to open (create) option transactions
- Show option positions in account overview
- Show option transactions in the transaction overview
- Add possibility to make screenshots via shortcuts and menu
- Add possibility to customize shortcuts

<!-- insertion marker -->
## [0.2.3](https://github.com/repo/owner/releases/tag/0.2.3) - 2026-05-17

### Bug Fix

Fix: Adding new stocks and saving does not yield two entries in securities overview page anymore

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











