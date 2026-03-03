#include "debug_slots_tree_view.hpp"

#include <QModelIndex>
#include <QVariant>

#include "logging/log_categories.hpp"
#include "logging/log_category.hpp"
#include "logging/log_macros.hpp"

namespace ui
{

    LogCategoryTreeModel::LogCategoryTreeModel(
        logging::LogCategories& categories,
        QObject*                parent
    )
        : QAbstractItemModel(parent), _categories(categories)
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

        const auto parentId = _getIdFromIndex(parent);
        const auto childId  = _getChildAt(parentId, row);

        if (childId == logging::InvalidLogCategoryId)
            return {};

        return createIndex(row, column, static_cast<quintptr>(childId));
    }

    QModelIndex LogCategoryTreeModel::parent(const QModelIndex& child) const
    {
        if (!child.isValid())
            return {};

        logging::LogCategoryId childId  = _getIdFromIndex(child);
        logging::LogCategoryId parentId = _getParentId(childId);

        if (parentId == logging::RootLogCategoryId ||
            parentId == logging::InvalidLogCategoryId)
            return {};

        // Find the row of the parent category among its siblings
        logging::LogCategoryId grandParentId = _getParentId(parentId);
        int                    row           = _getRowOfChild(
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
        const auto parentId = _getIdFromIndex(parent);

        if (parent.isValid() && parent.column() != 0)
            return 0;

        return static_cast<int>(_categories.getChildrenOf(parentId).size());
    }

    QVariant LogCategoryTreeModel::data(const QModelIndex& idx, int role) const
    {
        if (!idx.isValid())
            return {};

        const auto categoryId = _getIdFromIndex(idx);
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

    logging::LogCategoryId LogCategoryTreeModel::_getIdFromIndex(
        const QModelIndex& idx
    )
    {
        if (!idx.isValid())
            return logging::RootLogCategoryId;

        return static_cast<logging::LogCategoryId>(idx.internalId());
    }

    logging::LogCategoryId LogCategoryTreeModel::_getParentId(
        logging::LogCategoryId id
    ) const
    {
        const auto categoryOpt = _categories.getCategory(id);

        if (!categoryOpt.has_value())
            return logging::InvalidLogCategoryId;

        return categoryOpt->getParentId();
    }

    logging::LogCategoryId LogCategoryTreeModel::_getChildAt(
        logging::LogCategoryId parentId,
        int                    row
    ) const
    {
        const auto children = _categories.getChildrenOf(parentId);
        if (row < 0 || row >= static_cast<int>(children.size()))
            return logging::InvalidLogCategoryId;

        return children[static_cast<std::size_t>(row)];
    }

    int LogCategoryTreeModel::_getRowOfChild(
        logging::LogCategoryId parentId,
        logging::LogCategoryId childId
    ) const
    {
        const auto children = _categories.getChildrenOf(parentId);
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            if (children[i] == childId)
                return static_cast<int>(i);
        }

        return -1;
    }

}   // namespace ui