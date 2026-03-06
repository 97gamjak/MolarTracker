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

    /**
     * @brief Construct a new Log Category Model:: Log Category Model object
     *
     * @param categories
     * @param parent
     */
    LogCategoryModel::LogCategoryModel(
        logging::LogCategories& categories,
        QObject*                parent
    )
        : QAbstractItemModel(parent),
          _categories(categories),
          _referenceCategories(categories.copy())
    {
    }

    /**
     * @brief Get the number of columns in the model
     *
     * @param parent The parent index
     * @return int The number of columns
     */
    int LogCategoryModel::columnCount(
        const QModelIndex& /*parent*/
    ) const
    {
        return ColumnCount;   // add an extra column for leaf nodes
    }

    /**
     * @brief Set the data for the editor widget based on the model index
     *
     * @param editor The editor widget to set the data for
     * @param index The model index containing the data to set in the editor
     */
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

        MT_DEBUG << "Creating index for row " << row << ", column " << column
                 << ", parentId " << parentId << ", childId " << childId
                 << ", internal id " << parent.internalId() << "\n";

        if (childId == logging::InvalidLogCategoryId ||
            childId == logging::RootLogCategoryId ||
            parentId == logging::InvalidLogCategoryId)
            return {};

        return createIndex(row, column, static_cast<quintptr>(childId));
    }

    /**
     * @brief Get the parent index of a given child index
     *
     * @param child The model index of the child item
     * @return QModelIndex The model index of the parent item, or an invalid
     * index if the child is a root item or if the child index is invalid
     */
    QModelIndex LogCategoryModel::parent(const QModelIndex& child) const
    {
        if (!child.isValid())
            return {};

        logging::LogCategoryId childId  = _getIdFromIndex(child);
        logging::LogCategoryId parentId = _getParentId(childId);

        if (parentId == logging::RootLogCategoryId ||
            parentId == logging::InvalidLogCategoryId)
            return {};

        MT_DEBUG << "Finding parent for childId " << childId << ", parentId "
                 << parentId << "internal id " << child.internalId() << "\n";

        // Find the row of the parent category among its siblings
        logging::LogCategoryId grandParentId = _getParentId(parentId);

        int row = _getRowOfChild(
            grandParentId == logging::InvalidLogCategoryId
                ? logging::RootLogCategoryId
                : grandParentId,
            parentId
        );

        MT_DEBUG << "Parent row is " << row << " with grandParentId "
                 << grandParentId << "\n";

        if (row < 0)
            return {};

        return createIndex(row, 0, static_cast<quintptr>(parentId));
    }

    /**
     * @brief Get the number of rows (child items) under a given parent index
     *
     * @param parent The model index of the parent item
     * @return int The number of child items under the given parent index
     */
    int LogCategoryModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.column() > 0)
            return 0;

        const auto parentId = _getIdFromIndex(parent);

        if (parentId == logging::InvalidLogCategoryId)
            return 0;

        const auto rowCount =
            static_cast<int>(_getFilteredChildren(parentId).size());

        return rowCount;
    }

    /**
     * @brief Get the data for a given model index and role
     *
     * @param idx The model index for which to retrieve the data
     * @param role The role for which to retrieve the data (e.g., display, edit)
     * @return QVariant The data for the given index and role, or an empty
     * QVariant if the index is invalid or if the role is not handled
     */
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

    /**
     * @brief Set the data in the model based on the editor widget's current
     * value
     *
     * @param editor The editor widget containing the new value to set in the
     * model
     * @param model The model to update with the new value from the editor
     * @param index The model index to update with the new value
     */
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

    /**
     * @brief Get the header data for a given section, orientation, and role
     *
     * @param section The section index for which to retrieve the header data
     * @param orientation The orientation (horizontal or vertical) of the header
     * @param role The role for which to retrieve the header data (e.g.,
     * display)
     * @return QVariant The header data for the given section, orientation, and
     * role, or an empty QVariant if the role is not handled
     */
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

    /**
     * @brief Get the item flags for a given model index
     *
     * @param idx The model index for which to retrieve the item flags
     * @return Qt::ItemFlags The item flags for the given index, indicating
     * whether the item is enabled, selectable, editable, etc.
     */
    Qt::ItemFlags LogCategoryModel::flags(const QModelIndex& idx) const
    {
        if (!idx.isValid())
            return {};

        Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

        if (idx.column() == Column::Level)
            flags |= Qt::ItemIsEditable;

        return flags;
    }

    /**
     * @brief Get the column index for the log level column
     *
     * @return int The column index for the log level column
     */
    int LogCategoryModel::getLogLevelColumn() { return Column::Level; }

    /**
     * @brief Get the column index for the "Apply to Children" column
     *
     * @return int The column index for the "Apply to Children" column
     */
    int LogCategoryModel::getApplyToChildrenColumn()
    {
        return Column::ApplyToChildren;
    }

    /**
     * @brief Set whether to show only modified categories in the model, and
     * refresh the view accordingly
     *
     * @param showModifiedOnly True to show only modified categories, false to
     * show all categories
     */
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