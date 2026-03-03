#include "debug_slots_tree_view.hpp"

#include <QModelIndex>
#include <QVariant>

#include "logging/log_categories.hpp"
#include "logging/log_category.hpp"

namespace ui
{

    LogCategoryTreeModel::LogCategoryTreeModel(
        logging::LogCategories& reg,
        QObject*                parent
    )
        : QAbstractItemModel(parent), _categories(reg)
    {
    }

    QModelIndex LogCategoryTreeModel::index(
        int                row,
        int                column,
        const QModelIndex& parent
    ) const
    {
        if (row < 0 || column < 0 || column >= ColumnCount)
            return {};

        const auto parentId = id_from_index_(parent);
        const auto childId  = child_at_(parentId, row);

        if (childId == logging::InvalidLogCategoryId)
            return {};

        return createIndex(row, column, static_cast<quintptr>(childId));
    }

    QModelIndex LogCategoryTreeModel::parent(const QModelIndex& child) const
    {
        if (!child.isValid())
            return {};

        logging::LogCategoryId childId  = id_from_index_(child);
        logging::LogCategoryId parentId = parent_id_(childId);

        if (parentId == logging::RootLogCategoryId ||
            parentId == logging::InvalidLogCategoryId)
            return {};

        // Find the row of the parent category among its siblings
        logging::LogCategoryId grandParentId = parent_id_(parentId);
        int                    row           = row_of_child_(
            grandParentId == logging::InvalidLogCategoryId
                ? logging::RootLogCategoryId
                : grandParentId,
            parentId
        );

        if (row < 0)
            return {};

        return createIndex(row, 0, static_cast<quintptr>(parentId));
    }

    int LogCategoryTreeModel::rowCount(const QModelIndex& parent) const
    {
        const auto parentId = id_from_index_(parent);

        if (parent.isValid() && parent.column() != 0)
            return 0;

        return static_cast<int>(_categories.getChildrenOf(parentId).size());
    }

    QVariant LogCategoryTreeModel::data(const QModelIndex& idx, int role) const
    {
        if (!idx.isValid())
            return {};

        const auto categoryId = id_from_index_(idx);
        const auto category   = _categories.getCategory(categoryId);

        if (role == Qt::DisplayRole)
        {
            if (idx.column() == Column::Name)
                return QString::fromStdString(category->getName());
            else if (idx.column() == Column::Level)
                return QString::fromStdString(
                    std::string(LogLevelMeta::name(category->getLogLevel()))
                );
            else
                return {};
        }

        return {};
    }

    QVariant LogCategoryTreeModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if (section == Column::Name)
                return "Category";
            else if (section == Column::Level)
                return "Log Level";
        }
        return {};
    }

    Qt::ItemFlags LogCategoryTreeModel::flags(const QModelIndex& idx) const
    {
        if (!idx.isValid())
            return {};

        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (idx.column() == Column::Level)
            flags |= Qt::ItemIsEditable;

        return flags;
    }

}   // namespace ui