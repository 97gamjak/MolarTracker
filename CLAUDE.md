# CLAUDE.md — AI Assistant Guide for MolarTracker

This file documents the codebase structure, development workflows, and conventions that AI assistants must follow when contributing to MolarTracker.

---

## Project Overview

MolarTracker is a **C++23 desktop application** built with **Qt6** for dental (molar) and finance data tracking. It uses **SQLite3** as its database and **CMake** as its build system. Current version: `0.2.3`.

---

## Git Workflow — CRITICAL RULES

> **AI assistants must follow these rules without exception.**

1. **Never push directly to `main`, `master`, or `dev`.** These are protected branches.
2. **Always create a feature branch** from `dev` (or the relevant base branch) for any change.
3. **Open pull requests targeting `dev`**, not `main`, unless explicitly instructed otherwise.
4. Branch naming convention: `feature/<TICKET-ID>-short-description` or `fix/<TICKET-ID>-short-description`.
5. AI-generated branches must follow the pattern: `claude/<short-description>-<session-id>`.
6. Use `git push -u origin <branch-name>` when pushing a new branch.
7. **Never amend or force-push** to shared branches.
8. **Always fetch before branching** to ensure you start from an up-to-date base.

```bash
# Correct workflow
git fetch origin dev
git checkout -b feature/MOLTRACK-XX-my-feature origin/dev
# ... make changes, commit ...
git push -u origin feature/MOLTRACK-XX-my-feature
# Then open a PR targeting dev
```

---

## Repository Structure

```
MolarTracker/
├── .github/workflows/        # CI/CD pipelines (GitHub Actions)
├── .vscode/                  # VS Code editor configuration
├── cmake/                    # CMake helper modules and version logic
├── docs/                     # Project documentation
├── external/                 # Git submodule dependencies (json, mstd, devops)
├── scripts/                  # Build and utility scripts
├── src/                      # Main application source (~443 files, 18 modules)
│   ├── app/                  # Application layer (domain, repos, services, store, migration)
│   ├── config/               # Constants, type definitions, finance enums, error type
│   ├── connections/          # Observable/Signal event system
│   ├── db/                   # SQLite3 database wrapper
│   ├── drafts/               # Staging/draft objects (e.g. ProfileDraft)
│   ├── exceptions/           # Exception hierarchy
│   ├── filter/               # Predicate-based expression filtering
│   ├── finance/              # Finance domain types (Account, Cash, Stock, Transaction)
│   ├── http/                 # HTTP client (CURL-based, used for ticker data)
│   ├── json/                 # JSON serialization helpers
│   ├── logging/              # Ring-buffer file logger and log manager
│   ├── orm/                  # Template-based ORM (C++23 concepts)
│   ├── settings/             # JSON-persisted settings with parameter system
│   ├── sql_models/           # Database table row definitions
│   ├── ui/                   # Qt GUI layer (widgets, controllers, commands)
│   └── utils/                # Cross-platform utility functions
├── tests/                    # GoogleTest unit tests
├── CMakeLists.txt            # Root build configuration
├── Dockerfile                # Ubuntu-based development container
├── vcpkg.json                # vcpkg package dependencies (sqlite3, curl)
├── devops.toml               # DevOps tooling configuration
├── .clang-format             # Code formatting rules
├── .clang-tidy               # Static analysis rules
├── .clangd                   # Language server configuration
├── .cppcheck                 # cppcheck suppression configuration
├── .iwyu.imp                 # include-what-you-use mappings
├── CHANGELOG.md              # User-facing changelog
└── DEV-CHANGELOG.md          # Developer-facing changelog
```

### Module Layout Convention

Every source module under `src/` follows a consistent layout:

```
src/<module>/
├── CMakeLists.txt
├── include/<module>/      # Public headers (installed/exported)
│   └── *.hpp / *.tpp
└── src/<module>/          # Private implementation files
    └── *.cpp / *.hpp
```

---

## Build System

**Requirements:**
- CMake >= 3.20
- C++23-capable compiler (GCC or MSVC)
- Qt6 (Widgets, Core) — tested with Qt 6.8.3
- CURL
- vcpkg (for `sqlite3`, `curl`)
- Ninja (recommended generator)

**Build commands:**

```bash
# Configure (Linux)
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run tests
cd build && ctest --output-on-failure

# Build with documentation (optional)
cmake -S . -B build -DMOLARTRACKER_ENABLE_DOCS=ON

# Install desktop file (Linux only)
cmake --build build --target install-desktop-dev
```

**Key CMake options:**

| Option | Default | Description |
|---|---|---|
| `MOLARTRACKER_ENABLE_TESTING` | `ON` | Build unit tests |
| `MOLARTRACKER_ENABLE_DOCS` | `OFF` | Build Doxygen documentation |
| `BUILD_TESTING` | `ON` | CTest infrastructure |

**Version definitions** injected at compile time (from `cmake/version.cmake`):

| Macro | Description |
|---|---|
| `MOLARTRACKER_VERSION` | Semantic version string |
| `MOLARTRACKER_VERSION_FULL` | Full version including git metadata |
| `MOLARTRACKER_GIT_SHA` | Current commit SHA |
| `MOLARTRACKER_GIT_DIRTY` | Whether working tree is dirty |
| `MOLARTRACKER_GIT_TAG` | Current git tag |

---

## Source Code Architecture

The application follows a clean layered architecture:

```
UI (Qt Widgets)
    ↓
Controllers (MVC)
    ↓
Services (Business Logic)
    ↓
Repositories (Data Access)
    ↓
ORM / Database (SQLite3)
```

### Key Modules

**`src/app/`** — Application domain and orchestration
- `domain/` — Core entities (e.g. `Profile`)
- `repos_api/` — Repository interfaces (`IProfileRepo`, `IAccountRepo`, `IInstrumentRepo`, `ITransactionRepo`)
- `services_api/` — Service interfaces (`IProfileService`, `IAccountService`, `IInstrumentService`, `ITransactionService`)
- `store/` — Global application state; `ProfileStore`, `AccountStore`, `StockStore`, `TransactionStore`; base store template in `store/base/`
- `store/profile/` — `ActiveProfile` observable type
- `migration/` — Database migration system (`Migration`, `MigrationRunner`, `SingleMigration`, `MultiMigration`)
- `factories/` — Object construction (`ProfileFactory`, `AccountFactory`, `InstrumentFactory`, `TransactionFactory`)
- `app_context.hpp` — Aggregates `RepoContainer`, `ServiceContainer`, `StoreContainer`

**`src/finance/`** — Finance domain types
- `Account` — Account with `std::variant` details (cash or security)
- `Cash` — Amount + currency value type
- `Stock` / `TickerInfo` — Security/instrument representation
- `Transaction` / `TransactionEntry` — Double-entry style transactions
- `TradeData` — Trade leg data
- `YfClient` — Yahoo Finance HTTP client (wraps `http/`)
- `Currency` helpers (also see `config/finance.hpp` for enums)

**`src/http/`** — HTTP client abstraction (CURL-based)
- `HttpClient` / `HttpRequest` / `HttpResponse` — Generic HTTP interface
- `Curl` — CURL backend implementation
- `HttpError` — Error type for HTTP operations

**`src/filter/`** — Predicate expression system
- `ExprNode` / `Predicate` — Composable boolean expression nodes
- `Operators` — Logical operator helpers

**`src/ui/`** — Qt GUI layer
- `account/` — Account overview and detail pages
- `central/` — Central widget / content area
- `securities/` — Securities overview page, add-security dialog
- `side_bar/` — Application sidebar navigation
- `transaction/` — Transaction creation dialogs, deposit/withdrawal widgets
- `profile/` — Profile management dialogs
- `settings/` — Settings dialog
- `logging/` — Log file viewer widget
- `menu_bar/` — Menu bar and actions
- `base/` — Shared widget base classes
- `validators/` — Qt input validators
- `utils/` — UI utility helpers
- `exceptions/` — UI-layer exceptions
- `application.hpp` — Qt application wrapper
- `main_window.hpp` — Root window

**`src/db/`** — SQLite3 abstraction
- `database.hpp/cpp` — Connection management
- `statement.hpp/cpp` — Prepared statements
- `transaction.hpp/cpp` — Transaction RAII wrapper

**`src/orm/`** — Compile-time ORM (template-heavy, C++23 concepts)
- `OrmModel` — Base CRUD operations
- `WhereClause` / `WhereExpr` — Typed SQL WHERE expression builder
- `Join` — JOIN support
- `QueryOptions` — ORDER BY, LIMIT, OFFSET
- `Field` / `Fields` — Compile-time field descriptors
- `Binder` — SQLite3 parameter binding
- Constraint types: `not_null`, `unique`, `primary_key`
- `CrudError` — Typed error enum for CRUD failures

**`src/settings/`** — Persistent JSON settings
- `Settings` — Root settings object (version-tracked)
- `GeneralSettings` — Application-level settings
- `UISettings` — UI preferences
- `LogViewerSettings` — Log viewer window state
- `LoggingSettings` — Logger configuration
- `DebugSlotsSettings` — Debug slot selector state
- `ProfileUISettings` — Per-profile UI state
- Parameter types: `BoolParam`, `NumericParam`, `NumericVecParam`, `EnumParam`, `StringParam`, `VersionParam`
- `ParamContainer` / `ParamContainerMixin` — Group and iterate params

**`src/connections/`** — Event system
- `Observable<Events...>` — Multi-event subscriber base
- `Signal<Tag>` — Event emitter
- `Connection` — RAII subscription handle

**`src/logging/`** — Logging infrastructure
- `LogManager` singleton
- Ring-buffer file output (`RingFile`)
- Macros with category and scope support

**`src/config/`** — Application-wide types and constants
- `Constants` — Singleton for paths and app metadata
- `id_types.hpp` — All strong-typed ID aliases (see ID Types below)
- `signal_tags.hpp` — Event tag types (see Signal Tags below)
- `finance.hpp` — Finance enums (Currency, AccountKind, etc.)
- `error.hpp` — Generic `Error<EnumType>` for `std::expected` patterns
- `strong_id.hpp` — `StrongId<Tag>` template, `IdSequence`, `unorderedIdMap`
- `quantity.hpp` — Numeric quantity type
- `type_traits.hpp` — Project-wide type trait helpers
- `logging_base.hpp` — Logging category base types

---

## ID Types

All IDs are strongly typed via `StrongId<Tag>` to prevent accidental integer mixing. Defined in `src/config/include/config/id_types.hpp`:

| Type | Tag | Domain |
|---|---|---|
| `ProfileId` | `ProfileTag` | User profiles |
| `SecurityId` | `SecurityTag` | Generic securities |
| `InstrumentId` | `InstrumentTag` | Financial instruments |
| `StockId` | `StockTag` | Stock securities |
| `AccountId` | `AccountTag` | Accounts |
| `TransactionId` | `TransactionTag` | Transactions |
| `TransactionEntryId` | `TransactionEntryTag` | Transaction line items |
| `TradeLegId` | `TradeLegTag` | Trade legs |
| `PositionId` | `PositionTag` | Positions |

Additional aliases: `InstrumentIdSeq` (`IdSequence<InstrumentId>`), `instrumentMap<T>` (`unorderedIdMap<InstrumentId, T>`).

When adding a new domain entity, define its ID here as a new `StrongId` specialization.

---

## Finance Enums

Defined in `src/config/include/config/finance.hpp` via `MSTD_ENUM` macro:

| Enum | Values |
|---|---|
| `Currency` | `USD`, `EUR`, `GBP`, `CHF` |
| `AccountKind` | `Cash`, `Security`, `External` |
| `AccountStatus` | `Active`, `Closed` |
| `TransactionStatus` | `Completed`, `Deleted` |
| `TransactionType` | `Deposit`, `Withdrawal`, `Stock` |
| `TransactionDataType` | `Cash`, `Trade` |
| `AssetClass` | `Stock`, `Etf`, `Crypto`, `MutualFund`, `Future`, `Unknown` |

---

## Signal Tags

Defined in `src/config/include/config/signal_tags.hpp`. Used with `Signal<Tag>` / `Observable<Tags...>`:

| Tag | Callback signature | Description |
|---|---|---|
| `OnDirtyChanged` | `void(const bool& isDirty)` | Dirty state changed |
| `OnStoreItemAdded<T>` | `void(const vector<T>&)` | Items added to a store |
| `OnStoreItemUpdated<T>` | `void(const vector<T>&)` | Items updated in a store |
| `OnStoreItemRemoved<IdType>` | `void(const vector<IdType>&)` | Items removed from a store |
| `OnIdRemap<IdType>` | `void(const unorderedIdMap<IdType,IdType>&)` | IDs remapped (e.g. after save) |
| `StoreChanged<IdType>` | `void()` | Generic store change notification |
| `OnSaved` | `void()` | Settings or data saved |
| `OnProfileChanged` | `void(const optional<ProfileId>&)` | Active profile changed |

---

## Error Handling

The project uses `std::expected<T, Error<EnumType>>` for recoverable errors. `Error<EnumType>` is defined in `src/config/include/config/error.hpp` and is parameterized by an enum that satisfies `mstd::has_enum_meta`.

Usage pattern:
```cpp
std::expected<MyResult, Error<MyErrorEnum>> doSomething();
// On failure: Error{MyErrorEnum::SomeCase, "human readable message"}
```

Each domain (repos, services, ORM CRUD) defines its own error enum and returns typed `Error` values.

---

## Naming Conventions

| Element | Convention | Example |
|---|---|---|
| Classes / Structs | PascalCase | `ProfileFactory`, `LogManager` |
| Functions / Methods | camelCase | `getProfile()`, `loadSettings()` |
| Private members | `_camelCase` | `_profileRepo`, `_settings` |
| Constants | `UPPER_CASE` or `static inline constexpr` | `MAX_RETRY` |
| Namespaces | lowercase | `app`, `ui`, `db`, `orm` |
| Header guards | `__MODULE_PATH_HPP__` | `__CONFIG__INCLUDE__CONFIG__ID_TYPES_HPP__` |
| Template impls | `.tpp` files included at end of `.hpp` | `orm_model.tpp` |
| Interfaces | `I` prefix | `IProfileRepo`, `IAccountService` |

Header guards follow the full file path convention: `__<DIR1>__<DIR2>__...__<FILENAME>_HPP__`.

---

## Code Formatting

Formatting is enforced by **clang-format** (Google style base, customized):

- Indent: **4 spaces** (no tabs)
- Column limit: **80 characters**
- Brace style: **Allman** (`BreakBeforeBraces: Allman`)
- Namespace indentation: **All**
- No bin-packing of arguments or parameters (`BinPackArguments: false`)
- Trailing comments aligned (`AlignTrailingComments: true`)
- Consecutive macros/assignments/declarations aligned
- Short functions/lambdas allowed on single line; short `if`/loops are **not**
- `RemoveSemicolon: true`
- Line endings: **LF** (`\n`), not CRLF

Run before committing:

```bash
clang-format -i <file>
# or for the whole project:
find src tests -name "*.hpp" -o -name "*.cpp" -o -name "*.tpp" | xargs clang-format -i
```

---

## Compiler Warnings (Strict)

All warnings are treated as errors. GCC flags include:

```
-Wall -Wextra -Wpedantic -Werror
-Wshadow -Wconversion -Wnull-dereference -Wlogical-op
```

MSVC adds `/Zc:preprocessor` for standards conformance. Do not suppress warnings without a documented justification.

---

## Testing

Tests use **GoogleTest** (fetched via `FetchContent`).

Test locations:

```
tests/
├── CMakeLists.txt
├── db/
│   ├── test_database.cpp
│   ├── test_statement.cpp / test_statement.hpp
│   └── test_transaction.cpp
├── orm/
│   └── CMakeLists.txt
└── utils/
    ├── test_paths.cpp
    └── test_version.cpp
```

Run tests:

```bash
cd build && ctest --output-on-failure
```

When adding new features, add corresponding tests in `tests/` mirroring the source module structure.

---

## CI/CD Pipelines

All pipelines are in `.github/workflows/`:

| Workflow | Trigger | Purpose |
|---|---|---|
| `build.yml` | PRs, push to `dev`, tags | Linux build + CTest + cppcheck + clang-tidy |
| `build_windows.yml` | PRs, tags | Windows MSVC build + windeployqt packaging |
| `static-analysis.yml` | PRs | DevOps Python style checks (`devops.toml`) |
| `doxygen.yml` | PRs | Strict Doxygen documentation build |
| `changelog.yml` | PRs | Verify at least one changelog was updated |
| `check-pr-for-release-version.yml` | PRs | Validate release version format |
| `create-tag.yml` | Manual | Create a release tag and upload artifacts |
| `claude.yml` | PRs (opened) | Automated Claude PR review (skips `claude/*` branches) |

Artifact packaging produces versioned `.tar.gz` files (Linux) and installer packages (Windows). Tags trigger release uploads.

**Changelog CI note:** The `changelog.yml` check only requires that **one** of `CHANGELOG.md` or `DEV-CHANGELOG.md` is modified per PR — not necessarily both.

---

## Database & ORM

**Engine:** SQLite3
**File location:** `${DATA_PATH}/molar_tracker.db`

**Current tables:**

| Table | Row type | Key columns |
|---|---|---|
| `profile` | `ProfileRow` | `id` PK AUTOINCREMENT, `name` TEXT NOT NULL UNIQUE, `email` TEXT nullable |
| `account` | `AccountRow` | paired insert (account + external counterpart) |
| `instrument` | `InstrumentRow` | instrument base record |
| `stock` | `StockRow` | stock security linked to instrument |
| `transaction_` | `TransactionRow` | `transaction_` not `transaction` (SQL reserved word) |
| `transaction_entry` | `TransactionEntryRow` | line items for a transaction |
| `trade_leg` | `TradeLegRow` | individual trade legs |
| `position` | `PositionRow` | open position tracking |

**ORM conventions:**
- Row types (e.g. `ProfileRow`) define compile-time field descriptors in `src/sql_models/`.
- Strong-typed IDs prevent accidental integer mixing.
- ORM handles CRUD generically via C++23 concepts (`OrmModel` concept in `src/orm/`).
- `WhereClause` / `WhereExpr` build type-safe WHERE conditions.
- `Join` supports table joins within ORM queries.
- Transactions are RAII-scoped via `src/db/transaction.hpp`.
- Note: `account` rows are always inserted in pairs (cash/security account + its external counterpart).

---

## Design Patterns in Use

| Pattern | Where |
|---|---|
| MVC | `src/ui/` controllers + widgets |
| Repository + Interface | `src/app/repos_api/` + `src/app/src/app/repos/` |
| Service Layer + Interface | `src/app/services_api/` + `src/app/src/app/services/` |
| Factory | `src/app/src/app/factories/` |
| Command (undo/redo) | `src/ui/` commands |
| Observable / Signal | `src/connections/` |
| Singleton | `Constants`, `LogManager` |
| Store (global state) | `src/app/include/app/store/` |
| Draft / Staging | `src/drafts/` |
| RAII | Transactions, connections, file handles |
| std::expected + Error | Services, repos, HTTP client |
| Expression Tree | `src/filter/` |

---

## Changelogs

Two changelogs must be updated for every meaningful change:

- **`CHANGELOG.md`** — User-facing, describes behavior changes and new features.
- **`DEV-CHANGELOG.md`** — Developer-facing, describes API/internal changes.

Both use `<!-- insertion marker -->` to indicate where new entries should be inserted. The `changelog.yml` CI check enforces that at least one changelog is updated in every PR.

---

## External Dependencies (Submodules)

Located in `external/`:

| Submodule | Source | Purpose |
|---|---|---|
| `json` | `github.com/nlohmann/json` | JSON library |
| `mstd` | `github.com/97gamjak/mstd` | Internal C++ standard library extensions (includes `MSTD_ENUM`) |
| `devops` | `github.com/97gamjak/devops` | Internal DevOps tooling (runs `static-analysis.yml`) |

Always clone with `--recurse-submodules` or run `git submodule update --init --recursive` after cloning.

---

## Development Environment

**Docker:**
```bash
docker build -t molartracker-dev .
docker run -it molartracker-dev
```

The Dockerfile is based on **Ubuntu 24.04** and installs all build dependencies.

**VSCode:** Configuration is in `.vscode/`. Recommended extensions include CMake Tools, clangd, and C/C++.

**Additional tooling files:**
- `.clangd` — Language server (clangd) configuration
- `.iwyu.imp` — include-what-you-use pragma mappings
- `.cppcheck` — cppcheck suppression configuration
- `devops.toml` — DevOps checks configuration (header guard rules, changelog paths, license header settings)

---

## Summary Checklist for AI Assistants

Before submitting any change:

- [ ] Branch created from `dev` (not `main`)
- [ ] PR targets `dev` (not `main`)
- [ ] Code formatted with `clang-format`
- [ ] No new compiler warnings introduced
- [ ] Tests added or updated for changed logic (in `tests/` mirroring module structure)
- [ ] At least one of `CHANGELOG.md` or `DEV-CHANGELOG.md` updated
- [ ] Git submodules not accidentally modified
- [ ] No hardcoded paths — use `Constants` singleton for app paths
- [ ] Private members prefixed with `_`
- [ ] New IDs defined as `StrongId` specializations in `src/config/include/config/id_types.hpp`
- [ ] New finance enums added to `src/config/include/config/finance.hpp`
- [ ] New signal tags added to `src/config/include/config/signal_tags.hpp`
- [ ] Header guards follow the full-path convention `__<DIR>__...__<FILE>_HPP__`
- [ ] New modules follow the `include/<module>/` + `src/<module>/` layout
- [ ] Error types use `Error<EnumType>` with `std::expected` (not raw exceptions for recoverable errors)
