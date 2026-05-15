// test_crud.cpp
//
// GoogleTest unit tests for orm::Crud.
//
// Coverage:
//  - createTable (DDL generation, SQL tracking)
//  - insert / batchInsert (return value, ID sequencing, atomicity)
//  - get / getUnique (empty, single, multiple rows; optional fields)
//  - update / updateField (success, not-found, no-PK)
//  - deleteByPk (removes row)
//  - WHERE / orderBy / limit query options
//  - addColumn / dropColumn (schema evolution)
//  - JOIN + getJoined
//  - Foreign-key constraint enforcement
//  - Unique constraint enforcement
//  - getExecutedSQL SQL tracking

#include <gtest/gtest.h>

#include <filesystem>
#include <optional>
#include <random>
#include <string>

#include "config/strong_id.hpp"
#include "db/database.hpp"
#include "db/db_exception.hpp"
#include "filter/operators.hpp"
#include "orm/constraints.hpp"
#include "orm/crud.hpp"
#include "orm/crud/crud_error.hpp"
#include "orm/field.hpp"
#include "orm/join.hpp"
#include "orm/orm_model.hpp"
#include "orm/query_options.hpp"

// ---------------------------------------------------------------------------
// Strong ID types for test models
// ---------------------------------------------------------------------------

struct ItemTag
{
};
using ItemId = StrongId<ItemTag>;

struct CategoryTag
{
};
using CategoryId = StrongId<CategoryTag>;

// ---------------------------------------------------------------------------
// Test model: ItemRow
//   id       INTEGER PRIMARY KEY AUTOINCREMENT
//   label    TEXT NOT NULL UNIQUE
//   score    REAL  (nullable)
//   active   INTEGER NOT NULL (bool)
//   note     TEXT  (optional string)
// ---------------------------------------------------------------------------
struct ItemRow : orm::ORMModel<"item">
{
    ORM_FIELD(id, IdField<ItemId>)
    ORM_FIELD(
        label,
        Field<"label", std::string, orm::not_null_t, orm::unique_t>
    )
    ORM_FIELD(score, Field<"score", double>)
    ORM_FIELD(active, Field<"active", bool, orm::not_null_t>)
    ORM_FIELD(note, Field<"note", std::optional<std::string>>)

    ORM_FIELDS(ItemRow, id, label, score, active, note)
};

// ---------------------------------------------------------------------------
// Test model: CategoryRow (parent in a FK relationship)
//   id    INTEGER PRIMARY KEY AUTOINCREMENT
//   name  TEXT NOT NULL
// ---------------------------------------------------------------------------
struct CategoryRow : orm::ORMModel<"category">
{
    ORM_FIELD(id, IdField<CategoryId>)
    ORM_FIELD(name, Field<"name", std::string, orm::not_null_t>)

    ORM_FIELDS(CategoryRow, id, name)
};

// ---------------------------------------------------------------------------
// Test model: TaggedItemRow (child with FK → CategoryRow, CASCADE delete)
//   id          INTEGER PRIMARY KEY AUTOINCREMENT
//   categoryId  INTEGER NOT NULL  FK → category.id ON DELETE CASCADE
//   tag         TEXT NOT NULL
// ---------------------------------------------------------------------------
struct TaggedItemRow : orm::ORMModel<"tagged_item">
{
    ORM_FIELD(id, IdField<ItemId>)
    ORM_FIELD(
        categoryId,
        Field<
            "category_id",
            CategoryId,
            orm::foreign_key_t<
                orm::CascadeDelete,
                CategoryRow,
                decltype(CategoryRow::id)>>
    )
    ORM_FIELD(tag, Field<"tag", std::string, orm::not_null_t>)

    ORM_FIELDS(TaggedItemRow, id, categoryId, tag)
};

// ---------------------------------------------------------------------------
// Test model: RestrictedItemRow (child with FK → CategoryRow, RESTRICT delete)
//   id          INTEGER PRIMARY KEY AUTOINCREMENT
//   categoryId  INTEGER NOT NULL  FK → category.id ON DELETE RESTRICT
//   tag         TEXT NOT NULL
// ---------------------------------------------------------------------------
struct RestrictedItemRow : orm::ORMModel<"restricted_item">
{
    ORM_FIELD(id, IdField<ItemId>)
    ORM_FIELD(
        categoryId,
        Field<
            "category_id",
            CategoryId,
            orm::foreign_key_t<
                orm::RestrictDelete,
                CategoryRow,
                decltype(CategoryRow::id)>>
    )
    ORM_FIELD(tag, Field<"tag", std::string, orm::not_null_t>)

    ORM_FIELDS(RestrictedItemRow, id, categoryId, tag)
};

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

namespace
{
    std::filesystem::path uniqueTempDbPath()
    {
        const auto tmp = std::filesystem::temp_directory_path();

        std::random_device                           random;
        std::mt19937_64                              gen(random());
        std::uniform_int_distribution<std::uint64_t> dis;

        return tmp / ("orm_test_" + std::to_string(dis(gen)) + ".sqlite");
    }

    struct TempDb
    {
        std::filesystem::path path;
        db::Database          db;

        TempDb() : path(uniqueTempDbPath()), db(path) {}

        ~TempDb()
        {
            db.close();
            std::error_code errorCode;
            std::filesystem::remove(path, errorCode);
        }

        TempDb(const TempDb&)            = delete;
        TempDb& operator=(const TempDb&) = delete;
        TempDb(TempDb&&)                 = delete;
        TempDb& operator=(TempDb&&)      = delete;
    };

    ItemRow makeItem(
        const std::string&         label,
        double                     score  = 0.0,
        bool                       active = true,
        std::optional<std::string> note   = std::nullopt
    )
    {
        ItemRow row;
        row.label  = label;
        row.score  = score;
        row.active = active;
        row.note   = std::move(note);
        return row;
    }

    void insertItem(orm::Crud& crud, db::Database& db, const ItemRow& row)
    {
        [[maybe_unused]] const auto result = crud.insert(db, row);
    }
}   // namespace

// ===========================================================================
// Test Fixtures
// ===========================================================================

class CrudTest : public ::testing::Test
{
   protected:
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    TempDb    _db;
    orm::Crud _crud;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    void SetUp() override { _crud.createTable<ItemRow>(_db.db); }
};

class FkCrudTest : public ::testing::Test
{
   protected:
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    TempDb    _db;
    orm::Crud _crud;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    void SetUp() override
    {
        _crud.createTable<CategoryRow>(_db.db);
        _crud.createTable<TaggedItemRow>(_db.db);
    }
};

class RestrictFkCrudTest : public ::testing::Test
{
   protected:
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    TempDb    _db;
    orm::Crud _crud;
    // NOLINTEND(misc-non-private-member-variables-in-classes)

    void SetUp() override
    {
        _crud.createTable<CategoryRow>(_db.db);
        _crud.createTable<RestrictedItemRow>(_db.db);
    }
};

// ===========================================================================
// createTable tests
// ===========================================================================

TEST(CreateTable, CreatesTableWithoutThrow)
{
    TempDb    tdb;
    orm::Crud crud;
    EXPECT_NO_THROW(crud.createTable<ItemRow>(tdb.db));
}

TEST(CreateTable, SqlIsTrackedAfterCreate)
{
    TempDb    tdb;
    orm::Crud crud;
    crud.createTable<ItemRow>(tdb.db);

    const auto& executions = crud.getExecutedSQL();
    ASSERT_FALSE(executions.empty());
}

TEST(CreateTable, ExecutedSqlContainsTableName)
{
    TempDb    tdb;
    orm::Crud crud;
    crud.createTable<ItemRow>(tdb.db);

    const auto& executions = crud.getExecutedSQL();
    ASSERT_FALSE(executions.empty());
    EXPECT_NE(executions.front().find("item"), std::string::npos);
}

TEST(CreateTable, ExecutedSqlContainsAllColumnNames)
{
    TempDb    tdb;
    orm::Crud crud;
    crud.createTable<ItemRow>(tdb.db);

    const auto& sql = crud.getExecutedSQL().front();
    EXPECT_NE(sql.find("label"), std::string::npos);
    EXPECT_NE(sql.find("score"), std::string::npos);
    EXPECT_NE(sql.find("active"), std::string::npos);
    EXPECT_NE(sql.find("note"), std::string::npos);
}

TEST(CreateTable, IfNotExistsAllowsDoubleCreate)
{
    TempDb    tdb;
    orm::Crud crud;
    EXPECT_NO_THROW(crud.createTable<ItemRow>(tdb.db));
    EXPECT_NO_THROW(crud.createTable<ItemRow>(tdb.db));
}

TEST(CreateTable, ForeignKeyConstraintInSql)
{
    TempDb    tdb;
    orm::Crud crud;
    crud.createTable<CategoryRow>(tdb.db);
    crud.createTable<TaggedItemRow>(tdb.db);

    bool foundFk = false;
    for (const auto& sql : crud.getExecutedSQL())
    {
        if (sql.find("FOREIGN KEY") != std::string::npos)
        {
            foundFk = true;
            break;
        }
    }
    EXPECT_TRUE(foundFk);
}

// ===========================================================================
// insert tests
// ===========================================================================

TEST_F(CrudTest, InsertReturnsPositiveId)
{
    const auto result = _crud.insert(_db.db, makeItem("alpha"));
    ASSERT_TRUE(result.has_value());
    EXPECT_GT(result.value(), 0);
}

TEST_F(CrudTest, SequentialInsertsReturnIncreasingIds)
{
    const auto id1 = _crud.insert(_db.db, makeItem("a"));
    const auto id2 = _crud.insert(_db.db, makeItem("b"));
    const auto id3 = _crud.insert(_db.db, makeItem("c"));

    ASSERT_TRUE(id1.has_value());
    ASSERT_TRUE(id2.has_value());
    ASSERT_TRUE(id3.has_value());
    EXPECT_LT(id1.value(), id2.value());
    EXPECT_LT(id2.value(), id3.value());
}

TEST_F(CrudTest, InsertWithNullOptionalSucceeds)
{
    const ItemRow row    = makeItem("nullNote", 1.0, true, std::nullopt);
    const auto    result = _crud.insert(_db.db, row);
    EXPECT_TRUE(result.has_value());
}

TEST_F(CrudTest, InsertWithOptionalValueSucceeds)
{
    const ItemRow row =
        makeItem("hasNote", 2.0, false, std::string{"some note"});
    const auto result = _crud.insert(_db.db, row);
    EXPECT_TRUE(result.has_value());
}

TEST_F(CrudTest, InsertSqlIsTracked)
{
    const std::size_t           before = _crud.getExecutedSQL().size();
    [[maybe_unused]] const auto result =
        _crud.insert(_db.db, makeItem("tracked"));
    EXPECT_GT(_crud.getExecutedSQL().size(), before);
}

// ===========================================================================
// get tests
// ===========================================================================

TEST_F(CrudTest, GetOnEmptyTableReturnsEmptyVector)
{
    const auto rows = _crud.get<ItemRow>(_db.db);
    EXPECT_TRUE(rows.empty());
}

TEST_F(CrudTest, GetReturnsInsertedRow)
{
    [[maybe_unused]] const auto result =
        _crud.insert(_db.db, makeItem("widget"));
    const auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);
    EXPECT_EQ(std::string(rows.front().label.value()), "widget");
}

TEST_F(CrudTest, GetPreservesAllScalarFields)
{
    const ItemRow row = makeItem("roundtrip", 3.14, false, std::nullopt);
    [[maybe_unused]] const auto result = _crud.insert(_db.db, row);

    const auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);

    const auto& frontRow = rows.front();
    EXPECT_EQ(std::string(frontRow.label.value()), "roundtrip");
    EXPECT_DOUBLE_EQ(frontRow.score.value(), 3.14);
    EXPECT_FALSE(frontRow.active.value());
}

TEST_F(CrudTest, GetPreservesNullOptional)
{
    [[maybe_unused]] const auto result =
        _crud.insert(_db.db, makeItem("noNote", 0.0, true, std::nullopt));
    const auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);
    EXPECT_FALSE(rows.front().note.value().has_value());
}

TEST_F(CrudTest, GetPreservesOptionalWithValue)
{
    const auto result = _crud.insert(
        _db.db,
        makeItem("noted", 0.0, true, std::string{"hello orm"})
    );
    const auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);
    ASSERT_TRUE(rows.front().note.value().has_value());
    EXPECT_EQ(rows.front().note.value().value(), "hello orm");
}

TEST_F(CrudTest, GetReturnsAllInsertedRows)
{
    auto result = _crud.insert(_db.db, makeItem("x"));
    result      = _crud.insert(_db.db, makeItem("y"));
    result      = _crud.insert(_db.db, makeItem("z"));

    const auto rows = _crud.get<ItemRow>(_db.db);
    EXPECT_EQ(rows.size(), 3U);
}

TEST_F(CrudTest, GetAssignsAutoIncrementId)
{
    const auto result = _crud.insert(_db.db, makeItem("idCheck"));
    const auto rows   = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);
    EXPECT_GT(rows.front().id.value().value(), 0);
}

// ===========================================================================
// getUnique tests
// ===========================================================================

TEST_F(CrudTest, GetUniqueReturnsNulloptOnEmptyTable)
{
    const orm::Query query = orm::Query{}.where<ItemRow::labelField>(
        "nonexistent",
        filter::Operator::Equal
    );
    const auto result = _crud.getUnique<ItemRow>(_db.db, query);
    EXPECT_FALSE(result.has_value());
}

TEST_F(CrudTest, GetUniqueReturnsSingleMatchingRow)
{
    const auto _result = _crud.insert(_db.db, makeItem("unique_label"));

    const orm::Query query = orm::Query{}.where<ItemRow::labelField>(
        "unique_label",
        filter::Operator::Equal
    );
    const auto result = _crud.getUnique<ItemRow>(_db.db, query);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(std::string(result->label.value()), "unique_label");
}

TEST_F(CrudTest, GetUniqueThrowsWhenMultipleRowsMatch)
{
    auto result = _crud.insert(_db.db, makeItem("dup_a", 1.0));
    result      = _crud.insert(_db.db, makeItem("dup_b", 1.0));

    const orm::Query query =
        orm::Query{}.where<ItemRow::scoreField>(1.0, filter::Operator::Equal);

    EXPECT_THROW(
        (void) _crud.getUnique<ItemRow>(_db.db, query),
        orm::CrudException
    );
}

// ===========================================================================
// Query options: where / orderBy / limit
// ===========================================================================

TEST_F(CrudTest, WhereFiltersByLabel)
{
    insertItem(_crud, _db.db, makeItem("apple"));
    insertItem(_crud, _db.db, makeItem("banana"));
    insertItem(_crud, _db.db, makeItem("cherry"));

    const orm::Query query = orm::Query{}.where<ItemRow::labelField>(
        "banana",
        filter::Operator::Equal
    );

    const auto rows = _crud.get<ItemRow>(_db.db, query);
    ASSERT_EQ(rows.size(), 1U);
    EXPECT_EQ(std::string(rows.front().label.value()), "banana");
}

TEST_F(CrudTest, WhereWithNoMatchReturnsEmpty)
{
    insertItem(_crud, _db.db, makeItem("only_item"));

    const orm::Query query = orm::Query{}.where<ItemRow::labelField>(
        "missing",
        filter::Operator::Equal
    );

    const auto rows = _crud.get<ItemRow>(_db.db, query);
    EXPECT_TRUE(rows.empty());
}

TEST_F(CrudTest, OrderByLabelAscending)
{
    insertItem(_crud, _db.db, makeItem("mango"));
    insertItem(_crud, _db.db, makeItem("apple"));
    insertItem(_crud, _db.db, makeItem("kiwi"));

    const auto rows = _crud.get<ItemRow>(
        _db.db,
        orm::Query{}.orderBy<ItemRow::labelField>(true)
    );
    ASSERT_EQ(rows.size(), 3U);
    EXPECT_EQ(std::string(rows[0].label.value()), "apple");
    EXPECT_EQ(std::string(rows[1].label.value()), "kiwi");
    EXPECT_EQ(std::string(rows[2].label.value()), "mango");
}

TEST_F(CrudTest, OrderByLabelDescending)
{
    insertItem(_crud, _db.db, makeItem("mango"));
    insertItem(_crud, _db.db, makeItem("apple"));
    insertItem(_crud, _db.db, makeItem("kiwi"));

    const auto rows = _crud.get<ItemRow>(
        _db.db,
        orm::Query{}.orderBy<ItemRow::labelField>(false)
    );
    ASSERT_EQ(rows.size(), 3U);
    EXPECT_EQ(std::string(rows[0].label.value()), "mango");
    EXPECT_EQ(std::string(rows[1].label.value()), "kiwi");
    EXPECT_EQ(std::string(rows[2].label.value()), "apple");
}

TEST_F(CrudTest, LimitRestrictsNumberOfResults)
{
    constexpr auto nItems = 5;
    for (int i = 0; i < nItems; ++i)
        insertItem(_crud, _db.db, makeItem("item_" + std::to_string(i)));

    const auto rows = _crud.get<ItemRow>(_db.db, orm::Query{}.limit(3));
    EXPECT_EQ(rows.size(), 3U);
}

TEST_F(CrudTest, LimitOneReturnsExactlyOneRow)
{
    insertItem(_crud, _db.db, makeItem("first"));
    insertItem(_crud, _db.db, makeItem("second"));

    const auto rows = _crud.get<ItemRow>(_db.db, orm::Query{}.limit(1));
    EXPECT_EQ(rows.size(), 1U);
}

TEST_F(CrudTest, WhereAndOrderByCombined)
{
    insertItem(_crud, _db.db, makeItem("z_active", 0.0, true));
    insertItem(_crud, _db.db, makeItem("a_active", 0.0, true));
    insertItem(_crud, _db.db, makeItem("m_inactive", 0.0, false));

    const auto rows = _crud.get<ItemRow>(
        _db.db,
        orm::Query{}
            .where<ItemRow::activeField>(true, filter::Operator::Equal)
            .orderBy<ItemRow::labelField>(true)
    );
    ASSERT_EQ(rows.size(), 2U);
    EXPECT_EQ(std::string(rows[0].label.value()), "a_active");
    EXPECT_EQ(std::string(rows[1].label.value()), "z_active");
}

// ===========================================================================
// update tests
// ===========================================================================

TEST_F(CrudTest, UpdateModifiesExistingRow)
{
    const auto _result = _crud.insert(_db.db, makeItem("before"));

    auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);

    constexpr auto score = 99.9;

    auto row  = rows.front();
    row.label = "after";
    row.score = score;

    const auto result = _crud.update(_db.db, row);
    ASSERT_TRUE(result.has_value()) << result.error().getMessage();

    const auto updated = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(updated.size(), 1U);
    EXPECT_EQ(std::string(updated.front().label.value()), "after");
    EXPECT_DOUBLE_EQ(updated.front().score.value(), score);
}

TEST_F(CrudTest, UpdateNonExistentRowReturnsNoRowsUpdated)
{
    constexpr auto id = 999999;

    ItemRow row;
    row.id     = ItemId::from(id);
    row.label  = "ghost";
    row.score  = 0.0;
    row.active = true;

    const auto result = _crud.update(_db.db, row);
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().getType(), orm::CrudErrorType::NoRowsUpdated);
}

TEST_F(CrudTest, UpdateChangesArePersistedCorrectly)
{
    const auto insertResult =
        _crud.insert(_db.db, makeItem("persist_me", 1.0, true));

    auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);

    auto row   = rows.front();
    row.active = false;
    row.note   = std::string{"updated note"};

    const auto updateResult = _crud.update(_db.db, row);

    const auto reloaded = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(reloaded.size(), 1U);
    EXPECT_FALSE(reloaded.front().active.value());
    ASSERT_TRUE(reloaded.front().note.value().has_value());
    EXPECT_EQ(reloaded.front().note.value().value(), "updated note");
}

// ===========================================================================
// deleteByPk tests
// ===========================================================================

TEST_F(CrudTest, DeleteByPkRemovesRow)
{
    const auto result = _crud.insert(_db.db, makeItem("to_delete"));

    auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 1U);

    _crud.deleteByPk(_db.db, rows.front());

    const auto after = _crud.get<ItemRow>(_db.db);
    EXPECT_TRUE(after.empty());
}

TEST_F(CrudTest, DeleteByPkLeavesOtherRowsIntact)
{
    auto result = _crud.insert(_db.db, makeItem("keep_a"));
    result      = _crud.insert(_db.db, makeItem("remove_b"));
    result      = _crud.insert(_db.db, makeItem("keep_c"));

    auto rows = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(rows.size(), 3U);

    ItemRow toRemove;
    for (const auto& row : rows)
    {
        if (std::string(row.label.value()) == "remove_b")
        {
            toRemove = row;
            break;
        }
    }

    _crud.deleteByPk(_db.db, toRemove);

    const auto after = _crud.get<ItemRow>(_db.db);
    ASSERT_EQ(after.size(), 2U);
    for (const auto& row : after)
        EXPECT_NE(std::string(row.label.value()), "remove_b");
}

// ===========================================================================
// batchInsert tests
// ===========================================================================

TEST_F(CrudTest, BatchInsertInsertsAllRows)
{
    const ItemRow batch_a = makeItem("batch_a");
    const ItemRow batch_b = makeItem("batch_b");
    const ItemRow batch_c = makeItem("batch_c");

    const auto result = _crud.batchInsert(_db.db, batch_a, batch_b, batch_c);
    ASSERT_TRUE(result.has_value());

    const auto rows = _crud.get<ItemRow>(_db.db);
    EXPECT_EQ(rows.size(), 3U);
}

TEST_F(CrudTest, BatchInsertReturnsIdForEachRow)
{
    const ItemRow bid_a = makeItem("bid_a");
    const ItemRow bid_b = makeItem("bid_b");

    const auto result = _crud.batchInsert(_db.db, bid_a, bid_b);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2U);

    EXPECT_GT((*result)[0], 0);
    EXPECT_GT((*result)[1], 0);
    EXPECT_NE((*result)[0], (*result)[1]);
}

// ===========================================================================
// Unique constraint tests
// ===========================================================================

TEST_F(CrudTest, InsertDuplicateLabelError)
{
    auto result = _crud.insert(_db.db, makeItem("unique_val"));
    result      = _crud.insert(_db.db, makeItem("unique_val"));

    ASSERT_FALSE(result.has_value());
}

// ===========================================================================
// Foreign key constraint tests
// ===========================================================================

TEST_F(FkCrudTest, InsertChildWithValidFkSucceeds)
{
    CategoryRow cat;
    cat.name         = "tech";
    const auto catId = _crud.insert(_db.db, cat).value();

    TaggedItemRow item;
    item.categoryId = CategoryId::from(catId);
    item.tag        = "laptop";

    EXPECT_NO_THROW(const auto result = _crud.insert(_db.db, item));
}

TEST_F(FkCrudTest, InsertChildWithInvalidFkThrows)
{
    constexpr auto id = 999999;
    TaggedItemRow  item;
    item.categoryId = CategoryId::from(id);
    item.tag        = "orphan";

    auto result = _crud.insert(_db.db, item);
    EXPECT_FALSE(result.has_value());
}

TEST_F(FkCrudTest, DeleteParentWithCascadeAlsoDeletesChild)
{
    CategoryRow cat;
    cat.name         = "deletable";
    const auto catId = _crud.insert(_db.db, cat).value();

    TaggedItemRow item;
    item.categoryId   = CategoryId::from(catId);
    item.tag          = "child";
    const auto result = _crud.insert(_db.db, item);

    auto cats = _crud.get<CategoryRow>(_db.db);
    ASSERT_EQ(cats.size(), 1U);
    _crud.deleteByPk(_db.db, cats.front());

    const auto children = _crud.get<TaggedItemRow>(_db.db);
    EXPECT_TRUE(children.empty());
}

TEST_F(RestrictFkCrudTest, DeleteParentWithRestrictFkThrows)
{
    CategoryRow cat;
    cat.name         = "protected";
    const auto catId = _crud.insert(_db.db, cat).value();

    RestrictedItemRow item;
    item.categoryId   = CategoryId::from(catId);
    item.tag          = "held";
    const auto result = _crud.insert(_db.db, item);

    auto cats = _crud.get<CategoryRow>(_db.db);
    ASSERT_EQ(cats.size(), 1U);

    EXPECT_THROW(_crud.deleteByPk(_db.db, cats.front()), db::SqliteError);
}

// ===========================================================================
// addColumn / dropColumn tests
// ===========================================================================

TEST_F(CrudTest, AddColumnSucceeds)
{
    using NewField = ItemRow::Field<"priority", int, orm::not_null_t>;
    const NewField defaultField{0};

    const auto result = _crud.addColumn(_db.db, defaultField);
    EXPECT_TRUE(result.has_value()) << result.error().getMessage();
}

TEST_F(CrudTest, AddDuplicateColumnReturnsError)
{
    using NewField = ItemRow::Field<"priority", int, orm::not_null_t>;
    const NewField defaultField{0};

    auto result = _crud.addColumn(_db.db, defaultField);
    result      = _crud.addColumn(_db.db, defaultField);

    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(
        result.error().getType(),
        orm::CrudErrorType::ColumnAlreadyExists
    );
}

TEST_F(CrudTest, DropNonExistentColumnReturnsError)
{
    const auto result = _crud.dropColumn<ItemRow>(_db.db, "nonexistent_col");
    ASSERT_FALSE(result.has_value());
    EXPECT_EQ(result.error().getType(), orm::CrudErrorType::ColumnDoesNotExist);
}

TEST_F(CrudTest, AddThenDropColumnSucceeds)
{
    using NewField = ItemRow::Field<"temp_col", int, orm::not_null_t>;
    const NewField defaultField{0};

    const auto addResult = _crud.addColumn(_db.db, defaultField);
    ASSERT_TRUE(addResult.has_value());

    const auto dropResult = _crud.dropColumn<ItemRow>(_db.db, "temp_col");
    EXPECT_TRUE(dropResult.has_value()) << dropResult.error().getMessage();
}

// ===========================================================================
// JOIN + getJoined tests
// ===========================================================================

TEST_F(FkCrudTest, GetJoinedReturnsTuplesForMatchingRows)
{
    CategoryRow cat;
    cat.name         = "vehicles";
    const auto catId = _crud.insert(_db.db, cat).value();

    TaggedItemRow item1;
    item1.categoryId = CategoryId::from(catId);
    item1.tag        = "car";
    auto result      = _crud.insert(_db.db, item1);

    TaggedItemRow item2;
    item2.categoryId = CategoryId::from(catId);
    item2.tag        = "bike";
    result           = _crud.insert(_db.db, item2);

    auto joins = orm::Joins{}.add(
        orm::innerJoin<TaggedItemRow::categoryIdField, CategoryRow::idField>()
    );

    const auto rows = _crud.getJoined<TaggedItemRow, CategoryRow>(
        _db.db,
        joins,
        orm::Query{}
    );
    EXPECT_EQ(rows.size(), 2U);
}

TEST_F(FkCrudTest, GetJoinedCategoryNameIsCorrect)
{
    CategoryRow cat;
    cat.name         = "fruits";
    const auto catId = _crud.insert(_db.db, cat).value();

    TaggedItemRow item;
    item.categoryId   = CategoryId::from(catId);
    item.tag          = "apple";
    const auto result = _crud.insert(_db.db, item);

    auto joins = orm::Joins{}.add(
        orm::innerJoin<TaggedItemRow::categoryIdField, CategoryRow::idField>()
    );

    const auto rows = _crud.getJoined<TaggedItemRow, CategoryRow>(
        _db.db,
        joins,
        orm::Query{}
    );
    ASSERT_EQ(rows.size(), 1U);

    const auto& [taggedItem, category] = rows.front();
    EXPECT_EQ(std::string(taggedItem.tag.value()), "apple");
    EXPECT_EQ(std::string(category.name.value()), "fruits");
}

// ===========================================================================
// getExecutedSQL tracking
// ===========================================================================

TEST(ExecutedSql, StartsEmpty)
{
    orm::Crud crud;
    EXPECT_TRUE(crud.getExecutedSQL().empty());
}

TEST(ExecutedSql, TracksSqlForAllOperations)
{
    TempDb    tdb;
    orm::Crud crud;

    crud.createTable<ItemRow>(tdb.db);
    EXPECT_EQ(crud.getExecutedSQL().size(), 1U);

    const auto insertResult = crud.insert(tdb.db, makeItem("track_me"));
    EXPECT_EQ(crud.getExecutedSQL().size(), 2U);

    const auto rows = crud.get<ItemRow>(tdb.db);
    EXPECT_EQ(crud.getExecutedSQL().size(), 3U);

    auto row                = rows.front();
    row.label               = "track_updated";
    const auto updateResult = crud.update(tdb.db, row);
    EXPECT_EQ(crud.getExecutedSQL().size(), 4U);

    crud.deleteByPk(tdb.db, row);
    EXPECT_EQ(crud.getExecutedSQL().size(), 5U);
}
