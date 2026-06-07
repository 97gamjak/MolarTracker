# Plan: Financial Options Instrument Type

## Context

MolarTracker currently supports stocks as the only specific instrument type. This plan adds financial **options** (call/put contracts) as a second instrument type, following the established `InstrumentRow` + `StockRow` paired-insertion pattern that already exists. Option trades should also be recordable alongside stock trades, so `TransactionType::Option` is added in parallel.

---

## Fields

| Field | DB column | Type | Constraints |
|---|---|---|---|
| `id` | `id` | `OptionId` | PK, auto-increment |
| `instrumentId` | `instrument_id` | `InstrumentId` | FK → `instrument`, CascadeDelete, not_null |
| `underlyingTicker` | `underlying_ticker` | `std::string` | not_null |
| `optionType` | `option_type` | `OptionType` (Call/Put) | not_null |
| `strikePrice` | `strike_price` | `micro_units` | not_null |
| `expirationDate` | `expiration_date` | `Timestamp` | not_null |
| `currency` | `currency` | `Currency` | not_null |

**Unique group:** `(underlyingTicker, optionType, strikePrice, expirationDate)` — the combination of these four fields uniquely identifies a contract.

---

## Implementation Steps

### 1. Config & Enums — `src/config/include/config/finance.hpp`

- Add `X(Option)` to `ASSET_CLASS_LIST`.
- Add `X(Option)` to `TRANSACTION_TYPE_LIST`.
- Add a new `OptionType` enum:

```cpp
#define OPTION_TYPE_LIST(X) \
    X(Call)                 \
    X(Put)

MSTD_ENUM(OptionType, std::uint8_t, OPTION_TYPE_LIST);
```

### 2. Strong ID — `src/config/include/config/id_types.hpp`

```cpp
struct OptionTag {};
using OptionId = StrongId<OptionTag>;
```

### 3. SQL Model — `src/sql_models/include/sql_models/option_row.hpp` *(new)*

```cpp
struct OptionRow : public orm::ORMModel<"option">
{
    [[nodiscard]]
    static orm::WhereExpr hasUnderlying(const std::string& underlyingTicker);

    ORM_FIELD(id, IdField<OptionId>)
    ORM_FIELD(instrumentId, InstrumentRow::template ForeignId<tableName, orm::CascadeDelete>)
    ORM_FIELD(underlyingTicker, Field<"underlying_ticker", std::string, orm::not_null_t>)
    ORM_FIELD(optionType, Field<"option_type", OptionType, orm::not_null_t>)
    ORM_FIELD(strikePrice, Field<"strike_price", micro_units, orm::not_null_t>)
    ORM_FIELD(expirationDate, Field<"expiration_date", Timestamp, orm::not_null_t>)
    ORM_FIELD(currency, Field<"currency", Currency, orm::not_null_t>)

    ORM_FIELDS(OptionRow, id, instrumentId, underlyingTicker, optionType,
               strikePrice, expirationDate, currency)

    static auto getUniqueGroups()
    {
        return orm::unique_set(
            orm::unique_group<
                &OptionRow::underlyingTicker,
                &OptionRow::optionType,
                &OptionRow::strikePrice,
                &OptionRow::expirationDate>()
        );
    }
};
```

Also create `src/sql_models/src/sql_models/option_row.cpp` *(new)* implementing `hasUnderlying()`,
and add it to `src/sql_models/CMakeLists.txt`.

### 4. Domain Object — `src/finance/include/finance/instrument/option.hpp` *(new)*

Mirror `finance::Stock` exactly:

```cpp
class Option
{
    OptionId     _id;
    InstrumentId _instrumentId;
    std::string  _underlyingTicker;
    OptionType   _optionType;
    micro_units  _strikePrice;
    Timestamp    _expirationDate;
    Currency     _currency;

public:
    explicit Option(std::string underlyingTicker, OptionType optionType,
                    micro_units strikePrice, Timestamp expirationDate,
                    Currency currency);

    // [[nodiscard]] getters for all 7 fields
    // setters for id and instrumentId only (set after DB insert)
    [[nodiscard]] std::string toString() const;
};

struct [[nodiscard]] OptionInsertionResult
{
    OptionId     optionId;
    InstrumentId instrumentId;
};
```

Also create `src/finance/src/finance/instrument/option.cpp` *(new)* and add it to `src/finance/CMakeLists.txt`.

### 5. Repository Interface — `src/repo/include/repo/i_instrument_repo.hpp`

Add pure-virtual methods parallel to the stock ones:

```cpp
[[nodiscard]]
virtual std::vector<finance::Option> getOptions(
    const idSet<InstrumentId>& ids) = 0;

[[nodiscard]]
virtual std::optional<finance::Option> getOption(
    const std::string& underlyingTicker,
    OptionType         optionType,
    micro_units        strikePrice,
    Timestamp          expirationDate) = 0;

[[nodiscard]]
virtual finance::OptionInsertionResult addOption(
    const finance::Option& option) = 0;

[[nodiscard]]
virtual bool optionExists(
    const std::string& underlyingTicker,
    OptionType         optionType,
    micro_units        strikePrice,
    Timestamp          expirationDate) = 0;
```

### 6. Repository Implementation — `src/repo/src/repo/instrument_repo.hpp/.cpp`

Follow the exact pattern of the existing stock methods:

- Private `_getOptionRows(const idSet<InstrumentId>&)`.
- `getOptions()` — transform rows via `InstrumentFactory::toOption()`.
- `getOption()` — `orm::Crud::getUnique<OptionRow>()` with a `WhereExpr` on all four key fields.
- `addOption()` — paired insert: `InstrumentRow` first, then `OptionRow`; throw `RepositoryException` on failure (same pattern as `addStock()`).
- `optionExists()` — `get<OptionRow>()` + `!result.empty()`.

### 7. Factory — `src/repo/src/repo/factories/instrument_factory.hpp/.cpp`

Add two methods mirroring `fromStock` / `toStock`:

```cpp
static std::pair<InstrumentRow, OptionRow> fromOption(const finance::Option&);
static finance::Option toOption(const OptionRow&);
```

### 8. Service Interface & Implementation

`src/service/include/service/i_instrument_service.hpp` — add the same four abstract methods (getOptions, getOption, addOption, optionExists).

`src/service/src/service/instrument_service.hpp/.cpp` — delegate each method directly to the corresponding repo call.

### 9. Database Migration — `src/repo/src/repo/migration/migration.cpp`

- Determine the current last migration version N (last `_migrateVN()` call in the file).
- Add `_migrateV{N+1}()` containing a single `CreateTableMigration<OptionRow>()`.
- Group it into a new `_migrate_next_version()` method and call it from `Migrations::Migrations()`.
- Add `#include "sql_models/option_row.hpp"` to `migration.cpp`.
- Declare the new private methods in `src/repo/src/repo/migration/migration.hpp`.

---

## Files Changed

| File | Action |
|---|---|
| `src/config/include/config/finance.hpp` | Modify — add `OptionType` enum; add `Option` to `ASSET_CLASS_LIST` and `TRANSACTION_TYPE_LIST` |
| `src/config/include/config/id_types.hpp` | Modify — add `OptionId` |
| `src/sql_models/include/sql_models/option_row.hpp` | **New** |
| `src/sql_models/src/sql_models/option_row.cpp` | **New** |
| `src/sql_models/CMakeLists.txt` | Modify |
| `src/finance/include/finance/instrument/option.hpp` | **New** |
| `src/finance/src/finance/instrument/option.cpp` | **New** |
| `src/finance/CMakeLists.txt` | Modify |
| `src/repo/include/repo/i_instrument_repo.hpp` | Modify |
| `src/repo/src/repo/instrument_repo.hpp` | Modify |
| `src/repo/src/repo/instrument_repo.cpp` | Modify |
| `src/repo/src/repo/factories/instrument_factory.hpp` | Modify |
| `src/repo/src/repo/factories/instrument_factory.cpp` | Modify |
| `src/service/include/service/i_instrument_service.hpp` | Modify |
| `src/service/src/service/instrument_service.hpp` | Modify |
| `src/service/src/service/instrument_service.cpp` | Modify |
| `src/repo/src/repo/migration/migration.hpp` | Modify |
| `src/repo/src/repo/migration/migration.cpp` | Modify |
| `CHANGELOG.md` / `DEV-CHANGELOG.md` | Modify |

---

## Verification

1. **Build:** `cmake --build build` — must compile cleanly.
2. **Tests:** `cd build && ctest --output-on-failure` — all existing tests must pass; add new option tests in `tests/app/` mirroring `test_instrument_repo.cpp` and `test_instrument_service.cpp`.
3. **Manual:** launch the app and confirm a new `option` table is created on first run.
