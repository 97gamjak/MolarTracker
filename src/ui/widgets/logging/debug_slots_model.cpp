#include "debug_slots_model.hpp"

#include <QModelIndex>
#include <QVariant>

#include "custom_roles.hpp"
#include "logging/log_categories.hpp"
#include "logging/log_category.hpp"
#include "logging/log_macros.hpp"

namespace ui
{

    enum Column : std::uint8_t
    {
        Name,
        Level,
        ApplyToChildren,
        ColumnCount
    };

    LogCategoryModel::LogCategoryModel(
        logging::LogCategories& categories,
        QObject*                parent
    )
        : QAbstractItemModel(parent),
          _categories(categories),
          _referenceCategories(categories.copy())
    {
    }

    [[nodiscard]] int LogCategoryModel::columnCount(
        const QModelIndex& /*parent*/
    ) const
    {
        return ColumnCount;   // add an extra column for leaf nodes
    }

    QModelIndex LogCategoryModel::index(
        int                row,
        int                column,
        const QModelIndex& parent
    ) const
    {
        if (row < 0 || column < 0 || column >= ColumnCount)
            return {};

        const auto parentId = _getIdFromIndex(parent);
        const auto childId  = _getChildAt(parentId, row);

        if (childId == logging::InvalidLogCategoryId ||
            childId == logging::RootLogCategoryId ||
            parentId == logging::InvalidLogCategoryId)
            return {};

        return createIndex(row, column, static_cast<quintptr>(childId));
    }

    QModelIndex LogCategoryModel::parent(const QModelIndex& child) const
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

        int row = _getRowOfChild(
            grandParentId == logging::InvalidLogCategoryId
                ? logging::InvalidLogCategoryId
                : grandParentId,
            parentId
        );

        if (row < 0)
            return {};

        return createIndex(row, 0, static_cast<quintptr>(parentId));
    }

    int LogCategoryModel::rowCount(const QModelIndex& parent) const
    {
        const auto parentId = _getIdFromIndex(parent);

        if (parentId == logging::InvalidLogCategoryId)
            return 0;

        const auto rowCount =
            static_cast<int>(_getFilteredChildren(parentId).size());

        MT_DEBUG << "rowCount for parentId " << parentId << " is " << rowCount
                 << "\n";

        return rowCount;
    }

    QVariant LogCategoryModel::data(const QModelIndex& idx, int role) const
    {
        if (!idx.isValid())
            return {};

        const auto categoryId = _getIdFromIndex(idx);
        const auto category   = _categories.getCategory(categoryId);

        if (role == Qt::DisplayRole)
        {
            switch (idx.column())
            {
                case Column::Name:
                    return QString::fromStdString(category->getSegment());
                case Column::Level:
                    return QString::fromStdString(
                        std::string(LogLevelMeta::name(category->getLogLevel()))
                    );
                case Column::ApplyToChildren:
                    return category->hasChildren() ? "Apply to children" : "";
                default:
                    return {};
            }
        }

        if (role == CustomRoles::IsCascadeApply)
            return category->hasChildren();

        return {};
    }

    bool LogCategoryModel::setData(
        const QModelIndex& idx,
        const QVariant&    value,
        int                role
    )
    {
        if (!idx.isValid() || role != Qt::EditRole)
            return false;

        if (idx.column() != Column::Level)
            return false;

        const auto categoryId = _getIdFromIndex(idx);
        const auto category   = _categories.getCategory(categoryId);

        if (!category.has_value())
            return false;

        const auto levelOpt =
            LogLevelMeta::from_string(value.toString().toStdString());

        if (!levelOpt.has_value())
            return false;

        _categories.setLogLevel(category->getName(), levelOpt.value());

        emit dataChanged(idx, idx);

        return true;
    }

    QVariant LogCategoryModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        {
            if (section == Column::Name)
                return "Category";

            if (section == Column::Level)
                return "Log Level";

            if (section == Column::ApplyToChildren)
                return "Apply to children";
        }

        return {};
    }

    Qt::ItemFlags LogCategoryModel::flags(const QModelIndex& idx) const
    {
        if (!idx.isValid())
            return {};

        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (idx.column() == Column::Level)
            flags |= Qt::ItemIsEditable;

        return flags;
    }

    int LogCategoryModel::getLogLevelColumn() { return Column::Level; }

    int LogCategoryModel::getApplyToChildrenColumn()
    {
        return Column::ApplyToChildren;
    }

    void LogCategoryModel::setShowModifiedOnly(bool showModifiedOnly)
    {
        if (_showModifiedOnly == showModifiedOnly)
            return;

        _showModifiedOnly = showModifiedOnly;
        beginResetModel();
        endResetModel();
    }

    void LogCategoryModel::setCategories(
        const logging::LogCategories& categories
    )
    {
        _categories = categories;
        beginResetModel();
        endResetModel();
    }

    void LogCategoryModel::setReferenceCategories(
        const logging::LogCategories& referenceCategories
    )
    {
        _referenceCategories = referenceCategories;
    }

    void LogCategoryModel::applyToChildren(const QModelIndex& idx)
    {
        if (!idx.isValid() || idx.column() != Column::ApplyToChildren)
            return;

        const auto categoryId = _getIdFromIndex(idx);
        const auto category   = _categories.getCategory(categoryId);

        if (!category.has_value())
            return;

        const auto logLevel = category->getLogLevel();

        const auto descendants = _categories.getAllDescendantsOf(categoryId);

        for (const auto& descendantId : descendants)
        {
            _categories.setLogLevel(
                _categories.getCategory(descendantId)->getName(),
                logLevel
            );

            emit dataChanged(
                index(0, Column::Level, idx),
                index(rowCount(idx) - 1, Column::Level, idx)
            );
        }
    }

    logging::LogCategoryId LogCategoryModel::_getIdFromIndex(
        const QModelIndex& idx
    )
    {
        if (!idx.isValid())
            return logging::RootLogCategoryId;

        return static_cast<logging::LogCategoryId>(idx.internalId());
    }

    logging::LogCategoryId LogCategoryModel::_getParentId(
        logging::LogCategoryId id
    ) const
    {
        const auto categoryOpt = _categories.getCategory(id);

        if (!categoryOpt.has_value())
            return logging::RootLogCategoryId;

        return categoryOpt->getParentId();
    }

    logging::LogCategoryId LogCategoryModel::_getChildAt(
        logging::LogCategoryId parentId,
        int                    row
    ) const
    {
        const auto children = _getFilteredChildren(parentId);

        if (row < 0 || row >= static_cast<int>(children.size()))
            return logging::InvalidLogCategoryId;

        return children[static_cast<std::size_t>(row)];
    }

    int LogCategoryModel::_getRowOfChild(
        logging::LogCategoryId parentId,
        logging::LogCategoryId childId
    ) const
    {
        const auto children = _getFilteredChildren(parentId);
        for (std::size_t i = 0; i < children.size(); ++i)
        {
            if (children[i] == childId)
                return static_cast<int>(i);
        }

        return -1;
    }

    std::vector<logging::LogCategoryId> LogCategoryModel::_getFilteredChildren(
        logging::LogCategoryId parent
    ) const
    {
        auto children = _categories.getChildrenOf(parent);

        if (!_showModifiedOnly)
            return children;

        MT_DEBUG << "Filtering children of parentId " << parent
                 << ", total children: " << children.size() << "\n";

        std::vector<logging::LogCategoryId> filteredChildren;
        for (const auto& child : children)
        {
            const auto categoryOpt    = _categories.getCategory(child);
            const auto refCategoryOpt = _referenceCategories.getCategory(child);

            MT_DEBUG << "ChildId: " << child << ", categoryOpt: "
                     << (categoryOpt.has_value() ? categoryOpt->getName()
                                                 : "<invalid category>")
                     << ", refCategoryOpt: "
                     << (refCategoryOpt.has_value() ? refCategoryOpt->getName()
                                                    : "<invalid category>")
                     << "\n";

            if (!categoryOpt.has_value() || !refCategoryOpt.has_value())
                continue;

            MT_DEBUG << "Comparing log levels for childId " << child
                     << ": current level = "
                     << LogLevelMeta::name(categoryOpt->getLogLevel())
                     << ", reference level = "
                     << LogLevelMeta::name(refCategoryOpt->getLogLevel())
                     << "\n";

            if (categoryOpt->getLogLevel() != refCategoryOpt->getLogLevel())
            {
                filteredChildren.push_back(child);
            }
        }

        return filteredChildren;
    }

}   // namespace ui