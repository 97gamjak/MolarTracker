#ifndef __UI__WIDGETS__LOGGING__DEBUG_SLOTS_MODEL_HPP__
#define __UI__WIDGETS__LOGGING__DEBUG_SLOTS_MODEL_HPP__

#include <QAbstractItemModel>

#include "logging/log_categories.hpp"

class QObject;   // Forward declaration

namespace ui
{
    /**
     * @brief A model for the debug slots tree view, which displays the log
     * categories and their log levels. It allows editing the log levels and
     * applying changes to child categories.
     *
     */
    class LogCategoryModel final : public QAbstractItemModel
    {
        Q_OBJECT

       private:
        /// Reference to the log categories being displayed and edited
        logging::LogCategories& _categories;

        /// A copy of the reference categories at the time they were set
        logging::LogCategories _referenceCategories;

        /// Whether to show only modified categories compared to the reference
        bool _showModifiedOnly = false;

       public:
        explicit LogCategoryModel(
            logging::LogCategories& categories,
            QObject*                parent
        );

        [[nodiscard]] QModelIndex index(
            int                row,
            int                column,
            const QModelIndex& parent
        ) const override;

        [[nodiscard]] QModelIndex parent(
            const QModelIndex& child
        ) const override;

        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]] QVariant data(
            const QModelIndex& idx,
            int                role
        ) const override;

        [[nodiscard]] bool setData(
            const QModelIndex& idx,
            const QVariant&    value,
            int                role
        ) override;

        [[nodiscard]] QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;

        [[nodiscard]] Qt::ItemFlags flags(
            const QModelIndex& idx
        ) const override;

        [[nodiscard]] static int getLogLevelColumn();
        [[nodiscard]] static int getApplyToChildrenColumn();

        void setShowModifiedOnly(bool showModifiedOnly);
        void setCategories(const logging::LogCategories& categories);
        void setReferenceCategories(
            const logging::LogCategories& referenceCategories
        );

        void applyToChildren(const QModelIndex& idx);

       private:
        [[nodiscard]] static logging::LogCategoryId _getIdFromIndex(
            const QModelIndex& idx
        );

        [[nodiscard]] logging::LogCategoryId _getChildAt(
            logging::LogCategoryId parentId,
            int                    row
        ) const;

        [[nodiscard]] int _getRowOfChild(
            logging::LogCategoryId parentId,
            logging::LogCategoryId childId
        ) const;

        [[nodiscard]] logging::LogCategoryId _getParentId(
            logging::LogCategoryId id
        ) const;

        [[nodiscard]] std::vector<logging::LogCategoryId> _getFilteredChildren(
            logging::LogCategoryId parentId
        ) const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__DEBUG_SLOTS_MODEL_HPP__