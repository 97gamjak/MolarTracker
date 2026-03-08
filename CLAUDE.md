# CLAUDE.md — AI Assistant Guide for MolarTracker

This file documents the codebase structure, development workflows, and conventions that AI assistants must follow when contributing to MolarTracker.

---

## Project Overview

MolarTracker is a **C++23 desktop application** built with **Qt6** for dental (molar) data tracking. It uses **SQLite3** as its database and **CMake** as its build system. Current version: `0.0.3`.

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
├── src/                      # Main application source (~181 files)
│   ├── app/                  # Application layer (domain, repos, services, store)
│   ├── config/               # Constants singleton and type definitions
│   ├── connections/          # Observable/Signal event system
│   ├── db/                   # SQLite3 database wrapper
│   ├── drafts/               # Staging/draft objects (e.g. ProfileDraft)
│   ├── exceptions/           # Exception hierarchy
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
├── vcpkg.json                # vcpkg package dependencies
├── devops.toml               # DevOps tooling configuration
├── .clang-format             # Code formatting rules
├── .clang-tidy               # Static analysis rules
├── .cppcheck                 # cppcheck suppression configuration
├── CHANGELOG.md              # User-facing changelog
└── DEV-CHANGELOG.md          # Developer-facing changelog
```

---

## Build System

**Requirements:**
- CMake >= 3.20
- C++23-capable compiler (GCC or MSVC)
- Qt6 (Widgets, Core)
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
cd build && ctest

# Build with documentation (optional)
cmake -S . -B build -DMOLARTRACKER_ENABLE_DOCS=ON
```

**Key CMake options:**

| Option | Default | Description |
|---|---|---|
| `MOLARTRACKER_ENABLE_TESTING` | `ON` | Build unit tests |
| `MOLARTRACKER_ENABLE_DOCS` | `OFF` | Build Doxygen documentation |
| `BUILD_TESTING` | `ON` | CTest infrastructure |

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

### Key Layers

**`src/app/`** — Application domain
- `domain/` — Core entities (e.g. `Profile`)
- `repos/` — Repository pattern for data access
- `services/` — Business logic services
- `store/` — Global application state (store pattern)
- `factories/` — Object construction (e.g. `ProfileFactory`)

**`src/ui/`** — Qt GUI layer
- `widgets/` — Reusable Qt widgets (profile dialogs, menu bars, validators, log viewer)
- `controller/` — UI controllers per menu/feature area
- `commands/` — Command pattern for undo/redo (`UndoStack`)
- `application.hpp` — Qt application wrapper
- `main_window.hpp` — Root window

**`src/db/`** — SQLite3 abstraction
- `database.hpp/cpp` — Connection management
- `statement.hpp/cpp` — Prepared statements
- `transaction.hpp/cpp` — Transaction RAII wrapper

**`src/orm/`** — Compile-time ORM (template-heavy, C++23 concepts)
- Generic CRUD operations
- Field constraint types: `not_null`, `unique`
- Strongly-typed model concepts

**`src/settings/`** — Persistent settings
- JSON-backed settings file
- `Param...` types with subscription support
- Sections: `GeneralSettings`, `UISettings` (including `LogViewerSettings`), `LoggingSettings`
- Version tracking (`version`, `oldVersion`)

**`src/connections/`** — Event system
- `Observable<Events...>` — Multi-event subscriber base
- `Signal` — Event emitter
- `Connection` — RAII subscription handle

**`src/logging/`** — Logging infrastructure
- `LogManager` singleton
- Ring-buffer file output (`RingFile`)
- Macros with category and scope support

**`src/config/`** — Application-wide constants
- `Constants` singleton (paths, app name, version)
- `id_types.hpp` — Strong typedefs (e.g. `ProfileId`)
- `signal_tags.hpp` — Event tag types

---

## Naming Conventions

| Element | Convention | Example |
|---|---|---|
| Classes / Structs | PascalCase | `ProfileFactory`, `LogManager` |
| Functions / Methods | camelCase | `getProfile()`, `loadSettings()` |
| Private members | `_camelCase` | `_profileRepo`, `_settings` |
| Constants | `UPPER_CASE` or `static inline` | `MAX_RETRY`, `static inline constexpr` |
| Namespaces | lowercase | `app`, `ui`, `db`, `orm` |
| Header guards | `__MODULE_NAME_HPP__` | `__APP_PROFILE_HPP__` |
| Template impls | `.tpp` files | `orm_model.tpp` |

---

## Code Formatting

Formatting is enforced by **clang-format** (Google style base, customized):

- Indent: **4 spaces** (no tabs)
- Column limit: **80 characters**
- Brace style: **Allman** (`BreakBeforeBraces: Allman`)
- Namespace indentation: **All**
- No bin-packing of arguments or parameters
- Trailing comments aligned
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

Tests use **GoogleTest** (v1.17.0, fetched via `FetchContent`).

Test locations:

```
tests/
├── db/
│   ├── test_database.cpp
│   ├── test_statement.cpp
│   └── test_transaction.cpp
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
| `build.yml` | PRs, push to `dev`, tags | Linux build + CTest + cppcheck |
| `build_windows.yml` | PRs, tags | Windows build |
| `static-analysis.yml` | PRs | DevOps style checks |
| `doxygen.yml` | PRs | Doxygen documentation build |
| `changelog.yml` | PRs | Verify changelog entries exist |
| `check-pr-for-release-version.yml` | PRs | Validate release version format |
| `create-tag.yml` | Manual | Create a release tag |

Artifact packaging produces versioned `.tar.gz` files (Linux). Tags trigger release uploads.

---

## Database & ORM

**Engine:** SQLite3
**File location:** `${DATA_PATH}/molar_tracker.db`

**Current tables:**

| Table | Columns |
|---|---|
| `profile` | `id` (PK, AUTOINCREMENT), `name` (TEXT NOT NULL UNIQUE), `email` (TEXT, nullable) |

**ORM conventions:**
- Row types (e.g. `ProfileRow`) define fields using compile-time descriptors in `src/sql_models/`.
- Strong-typed IDs (`ProfileId`) prevent accidental integer mixing.
- ORM handles CRUD generically via C++23 concepts.
- Transactions are RAII-scoped via `src/db/transaction.hpp`.

---

## Design Patterns in Use

| Pattern | Where |
|---|---|
| MVC | `src/ui/controller/` + `src/ui/widgets/` |
| Repository | `src/app/repos/` |
| Service Layer | `src/app/services/` |
| Factory | `src/app/factories/` |
| Command (undo/redo) | `src/ui/commands/` |
| Observable / Signal | `src/connections/` |
| Singleton | `Constants`, `LogManager` |
| Store (global state) | `src/app/store/` |
| Draft / Staging | `src/drafts/` |
| RAII | Transactions, connections, file handles |

---

## Changelogs

Two changelogs must be updated for every meaningful change:

- **`CHANGELOG.md`** — User-facing, describes behavior changes and new features.
- **`DEV-CHANGELOG.md`** — Developer-facing, describes API/internal changes.

Both use `<!-- insertion marker -->` to indicate where new entries should be inserted. The `changelog.yml` CI check enforces that changelogs are updated in every PR.

---

## External Dependencies (Submodules)

Located in `external/`:

- `json` — JSON library
- `mstd` — Internal C++ standard library extensions
- `devops` — Internal DevOps tooling

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

---

## Summary Checklist for AI Assistants

Before submitting any change:

- [ ] Branch created from `dev` (not `main`)
- [ ] PR targets `dev` (not `main`)
- [ ] Code formatted with `clang-format`
- [ ] No new compiler warnings introduced
- [ ] Tests added or updated for changed logic
- [ ] Both `CHANGELOG.md` and `DEV-CHANGELOG.md` updated if applicable
- [ ] Git submodules not accidentally modified
- [ ] No hardcoded paths — use `Constants` singleton for app paths
- [ ] Private members prefixed with `_`
- [ ] New IDs defined as strong typedefs in `src/config/id_types.hpp`
