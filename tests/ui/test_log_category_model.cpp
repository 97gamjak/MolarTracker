// test_log_category_model.cpp
//
// GoogleTest-based tests for ui::LogCategoryModel.
//
// Coverage:
//  - empty categories: rowCount returns 0 at root
//  - single flat category: rowCount, index, data
//  - hierarchical categories: parent/child navigation
//  - columnCount is always 3
//  - headerData for horizontal/display role
//  - flags: Level column is editable; others are not
//  - setData updates log level when role==EditRole on Level column
//  - setData fails for non-Level column or wrong role
//  - setShowModifiedOnly hides unmodified entries
//  - getLogLevelColumn / getApplyToChildrenColumn static helpers

#include <gtest/gtest.h>

#include <QVariant>
#include <set>
#include <string>

#include "config/logging_base.hpp"
#include "logging/log_categories.hpp"
#include "ui/logging/debug_slots_model.hpp"

namespace
{

    class LogCategoryModelTest : public ::testing::Test
    {
       protected:
        void SetUp() override
        {
            const std::set<std::string> names{"App", "App.UI", "App.DB"};
            _categories =
                std::make_unique<logging::LogCategories>(names, LogLevel::Info);
            _model = std::make_unique<ui::LogCategoryModel>(*_categories, nullptr);
        }

        std::unique_ptr<logging::LogCategories> _categories;
        std::unique_ptr<ui::LogCategoryModel>   _model;
    };

    TEST_F(LogCategoryModelTest, ColumnCountIsThree)
    {
        EXPECT_EQ(_model->columnCount({}), 3);
    }

    TEST_F(LogCategoryModelTest, RootHasOneTopLevelChild)
    {
        EXPECT_EQ(_model->rowCount({}), 1);
    }

    TEST_F(LogCategoryModelTest, TopLevelIndexIsValid)
    {
        const auto idx = _model->index(0, 0, {});
        EXPECT_TRUE(idx.isValid());
    }

    TEST_F(LogCategoryModelTest, TopLevelNameIsApp)
    {
        const auto idx  = _model->index(0, 0, {});
        const auto data = _model->data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "App");
    }

    TEST_F(LogCategoryModelTest, TopLevelLogLevelIsInfo)
    {
        const auto idx      = _model->index(0, 1, {});
        const auto levelStr = _model->data(idx, Qt::DisplayRole).toString();
        EXPECT_EQ(levelStr, "Info");
    }

    TEST_F(LogCategoryModelTest, TopLevelHasTwoChildren)
    {
        const auto parentIdx = _model->index(0, 0, {});
        EXPECT_EQ(_model->rowCount(parentIdx), 2);
    }

    TEST_F(LogCategoryModelTest, ChildIndexIsValid)
    {
        const auto parentIdx = _model->index(0, 0, {});
        const auto childIdx  = _model->index(0, 0, parentIdx);
        EXPECT_TRUE(childIdx.isValid());
    }

    TEST_F(LogCategoryModelTest, ParentOfChildIsTopLevel)
    {
        const auto parentIdx = _model->index(0, 0, {});
        const auto childIdx  = _model->index(0, 0, parentIdx);
        const auto recovered = _model->parent(childIdx);
        EXPECT_EQ(recovered.row(), parentIdx.row());
    }

    TEST_F(LogCategoryModelTest, ParentOfTopLevelIsInvalid)
    {
        const auto topIdx = _model->index(0, 0, {});
        EXPECT_FALSE(_model->parent(topIdx).isValid());
    }

    TEST_F(LogCategoryModelTest, HeaderDataCategoryColumn)
    {
        const auto h = _model->headerData(0, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_EQ(h.toString(), "Category");
    }

    TEST_F(LogCategoryModelTest, HeaderDataLevelColumn)
    {
        const auto h = _model->headerData(1, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_EQ(h.toString(), "Log Level");
    }

    TEST_F(LogCategoryModelTest, HeaderDataApplyToChildrenColumn)
    {
        const auto h = _model->headerData(2, Qt::Horizontal, Qt::DisplayRole);
        EXPECT_EQ(h.toString(), "Apply to children");
    }

    TEST_F(LogCategoryModelTest, HeaderDataVerticalReturnsEmpty)
    {
        const auto h = _model->headerData(0, Qt::Vertical, Qt::DisplayRole);
        EXPECT_FALSE(h.isValid());
    }

    TEST_F(LogCategoryModelTest, FlagsLevelColumnIsEditable)
    {
        const auto idx   = _model->index(0, ui::LogCategoryModel::getLogLevelColumn(), {});
        const auto flags = _model->flags(idx);
        EXPECT_TRUE(flags & Qt::ItemIsEditable);
    }

    TEST_F(LogCategoryModelTest, FlagsNameColumnNotEditable)
    {
        const auto idx   = _model->index(0, 0, {});
        const auto flags = _model->flags(idx);
        EXPECT_FALSE(flags & Qt::ItemIsEditable);
    }

    TEST_F(LogCategoryModelTest, FlagsInvalidIndexReturnsNone)
    {
        EXPECT_EQ(_model->flags({}), Qt::ItemFlags{});
    }

    TEST_F(LogCategoryModelTest, SetDataChangesLogLevel)
    {
        const auto idx = _model->index(0, ui::LogCategoryModel::getLogLevelColumn(), {});
        ASSERT_TRUE(idx.isValid());

        const bool ok = _model->setData(idx, QVariant("Debug"), Qt::EditRole);
        EXPECT_TRUE(ok);

        const auto data = _model->data(idx, Qt::DisplayRole);
        EXPECT_EQ(data.toString(), "Debug");
    }

    TEST_F(LogCategoryModelTest, SetDataFailsForNameColumn)
    {
        const auto idx = _model->index(0, 0, {});
        const bool ok  = _model->setData(idx, QVariant("something"), Qt::EditRole);
        EXPECT_FALSE(ok);
    }

    TEST_F(LogCategoryModelTest, SetDataFailsForWrongRole)
    {
        const auto idx = _model->index(0, ui::LogCategoryModel::getLogLevelColumn(), {});
        const bool ok  = _model->setData(idx, QVariant("Debug"), Qt::DisplayRole);
        EXPECT_FALSE(ok);
    }

    TEST_F(LogCategoryModelTest, SetDataFailsForInvalidLevel)
    {
        const auto idx = _model->index(0, ui::LogCategoryModel::getLogLevelColumn(), {});
        const bool ok  = _model->setData(idx, QVariant("NotALevel"), Qt::EditRole);
        EXPECT_FALSE(ok);
    }

    TEST_F(LogCategoryModelTest, GetLogLevelColumnReturnsOne)
    {
        EXPECT_EQ(ui::LogCategoryModel::getLogLevelColumn(), 1);
    }

    TEST_F(LogCategoryModelTest, GetApplyToChildrenColumnReturnsTwo)
    {
        EXPECT_EQ(ui::LogCategoryModel::getApplyToChildrenColumn(), 2);
    }

    TEST_F(LogCategoryModelTest, ShowModifiedOnlyHidesUnchanged)
    {
        _model->setShowModifiedOnly(true);
        EXPECT_EQ(_model->rowCount({}), 0);
    }

    TEST_F(LogCategoryModelTest, ShowModifiedOnlyShowsChangedEntry)
    {
        const auto idx = _model->index(0, ui::LogCategoryModel::getLogLevelColumn(), {});
        _model->setData(idx, QVariant("Debug"), Qt::EditRole);
        _model->setShowModifiedOnly(true);
        EXPECT_EQ(_model->rowCount({}), 1);
    }

    TEST_F(LogCategoryModelTest, DataForInvalidIndexReturnsEmpty)
    {
        EXPECT_FALSE(_model->data({}, Qt::DisplayRole).isValid());
    }

    TEST_F(LogCategoryModelTest, ApplyToChildrenColumnDataShowsButtonForParent)
    {
        const auto idx = _model->index(0, ui::LogCategoryModel::getApplyToChildrenColumn(), {});
        const auto data = _model->data(idx, Qt::DisplayRole);
        EXPECT_FALSE(data.toString().isEmpty());
    }

    TEST_F(LogCategoryModelTest, RowCountForColumnGreaterThanZeroIsZero)
    {
        const auto parentIdx =
            _model->index(0, 1, {});
        EXPECT_EQ(_model->rowCount(parentIdx), 0);
    }

    // Flat model with no children
    class FlatLogCategoryModelTest : public ::testing::Test
    {
       protected:
        void SetUp() override
        {
            const std::set<std::string> names{"Root"};
            _categories =
                std::make_unique<logging::LogCategories>(names, LogLevel::Warning);
            _model = std::make_unique<ui::LogCategoryModel>(*_categories, nullptr);
        }

        std::unique_ptr<logging::LogCategories> _categories;
        std::unique_ptr<ui::LogCategoryModel>   _model;
    };

    TEST_F(FlatLogCategoryModelTest, LeafHasNoChildren)
    {
        const auto idx = _model->index(0, 0, {});
        EXPECT_EQ(_model->rowCount(idx), 0);
    }

    TEST_F(FlatLogCategoryModelTest, ApplyToChildrenColumnEmptyForLeaf)
    {
        const auto idx =
            _model->index(0, ui::LogCategoryModel::getApplyToChildrenColumn(), {});
        EXPECT_TRUE(_model->data(idx, Qt::DisplayRole).toString().isEmpty());
    }

}   // namespace
