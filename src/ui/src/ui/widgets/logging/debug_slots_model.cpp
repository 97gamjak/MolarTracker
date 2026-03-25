#include "ui/logging/debug_slots_model.hpp"

#include <QModelIndex>
#include <QVariant>

#include "logging/log_categories.hpp"
#include "logging/log_category.hpp"
#include "ui/logging/custom_roles.hpp"

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
     * @brief make a QModelIndex for a given row, column, and parent index
     *
     * @param row The row number of the item
     * @param column The column number of the item
     * @param parent The parent index of the item
     * @return QModelIndex The created model index for the item
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

        logging::LogCategoryId grandParentId = _getParentId(parentId);

        int row = _getRowOfChild(
            grandParentId == logging::InvalidLogCategoryId
                ? logging::RootLogCategoryId
                : grandParentId,
            parentId
        );

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
     * @brief Set the data in the model for a given index, value, and role
     *
     * @param idx The model index for which to set the data
     * @param value The new value to set in the model
     * @param role The role for which to set the data (e.g., edit)
     * @return bool True if the data was successfully set, false if the index
     * is invalid, if the role is not handled, or if the value is invalid for
     * the role
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

    /**
     * @brief Set the log categories in the model, replacing the current
     * categories with the given ones and refreshing the view
     *
     * @param categories The new log categories to set in the model
     */
    void LogCategoryModel::setCategories(
        const logging::LogCategories& categories
    )
    {
        _categories = categories;
        beginResetModel();
        endResetModel();
    }

    /**
     * @brief Set the reference log categories for tracking modifications, which
     * are used to determine which categories have been modified compared to the
     * reference state
     *
     * @param referenceCategories The log categories to set as the reference
     * state
     */
    void LogCategoryModel::setReferenceCategories(
        const logging::LogCategories& referenceCategories
    )
    {
        _referenceCategories = referenceCategories;
    }

    /**
     * @brief Apply the log level change to all child categories of the given
     * index in the tree view
     *
     * @param idx The model index of the category for which to apply the log
     * level change to its children
     */
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

    /**
     * @brief Get the log category ID from a given model index
     *
     * @param idx The model index for which to retrieve the log category ID
     * @return logging::LogCategoryId The log category ID associated with the
     * given index, or InvalidLogCategoryId if the index is invalid
     */
    logging::LogCategoryId LogCategoryModel::_getIdFromIndex(
        const QModelIndex& idx
    )
    {
        if (!idx.isValid())
            return logging::RootLogCategoryId;

        return static_cast<logging::LogCategoryId>(idx.internalId());
    }

    /**
     * @brief Get the parent log category ID for a given log category ID
     *
     * @param id The log category ID for which to retrieve the parent ID
     * @return logging::LogCategoryId The parent log category ID, or
     * RootLogCategoryId if the given ID is a root category or if the ID is
     * invalid
     */
    logging::LogCategoryId LogCategoryModel::_getParentId(
        logging::LogCategoryId id
    ) const
    {
        const auto categoryOpt = _categories.getCategory(id);

        if (!categoryOpt.has_value())
            return logging::RootLogCategoryId;

        return categoryOpt->getParentId();
    }

    /**
     * @brief Get the child log category ID at a given row under a specified
     * parent category ID
     *
     * @param parentId The log category ID of the parent category
     * @param row The row index of the child category to retrieve
     * @return logging::LogCategoryId The log category ID of the child at the
     * specified row under the given parent, or InvalidLogCategoryId if the row
     * index is out of bounds or if the parent ID is invalid
     */
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

    /**
     * @brief Get the row index of a child category under a specified parent
     * category ID
     *
     * @param parentId The log category ID of the parent category
     * @param childId The log category ID of the child category for which to
     * find the row index
     * @return int The row index of the child category under the given parent,
     * or -1 if the child is not found among the parent's children or if the
     * parent ID is invalid
     */
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

    /**
     * @brief Get the child log category IDs under a specified parent category
     * ID, optionally filtering to show only modified categories compared to a
     * reference state
     *
     * @param parentId The log category ID of the parent category
     * @return std::vector<logging::LogCategoryId> A vector of log category IDs
     * representing the child categories under the given parent, filtered based
     * on the _showModifiedOnly flag and the reference categories if applicable
     */
    std::vector<logging::LogCategoryId> LogCategoryModel::_getFilteredChildren(
        logging::LogCategoryId parentId
    ) const
    {
        auto children = _categories.getChildrenOf(parentId);

        if (!_showModifiedOnly)
            return children;

        std::vector<logging::LogCategoryId> filteredChildren;
        for (const auto& child : children)
        {
            const auto categoryOpt    = _categories.getCategory(child);
            const auto refCategoryOpt = _referenceCategories.getCategory(child);

            if (!categoryOpt.has_value() || !refCategoryOpt.has_value())
                continue;

            if (categoryOpt->getLogLevel() != refCategoryOpt->getLogLevel())
            {
                filteredChildren.push_back(child);
            }
        }

        return filteredChildren;
    }

}   // namespace ui