#ifndef __UI__WIDGETS__LOGGING__DEBUG_SLOTS_TREE_VIEW_HPP__
#define __UI__WIDGETS__LOGGING__DEBUG_SLOTS_TREE_VIEW_HPP__

#include <QAbstractItemModel>

#include "logging/log_categories.hpp"

class QObject;   // Forward declaration

namespace ui
{
    class LogCategoryTreeModel final : public QAbstractItemModel
    {
        Q_OBJECT

       public:
        enum Column
        {
            Name        = 0,
            Level       = 1,
            ColumnCount = 2
        };

        explicit LogCategoryTreeModel(
            logging::LogCategories& reg,
            QObject*                parent
        );

        QModelIndex index(
            int                row,
            int                column,
            const QModelIndex& parent
        ) const override;
        QModelIndex parent(const QModelIndex& child) const override;

        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override
        {
            Q_UNUSED(parent);
            return ColumnCount;
        }

        QVariant data(const QModelIndex& idx, int role) const override;
        QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;
        Qt::ItemFlags flags(const QModelIndex& idx) const override;

        // Optional: refresh after registry changes
        void reset_from_registry();

       private:
        logging::LogCategories& _categories;

        // helpers
        logging::LogCategoryId id_from_index_(const QModelIndex& idx) const;
        QModelIndex index_from_id_(logging::LogCategoryId id, int column) const;

        logging::LogCategoryId child_at_(
            logging::LogCategoryId parent,
            int                    row
        ) const;
        int row_of_child_(
            logging::LogCategoryId parent,
            logging::LogCategoryId child
        ) const;

        logging::LogCategoryId parent_id_(logging::LogCategoryId id) const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__DEBUG_SLOTS_TREE_VIEW_HPP__