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

       private:
        logging::LogCategories& _categories;

       public:
        enum Column
        {
            Name        = 0,
            Level       = 1,
            ColumnCount = 2
        };

        explicit LogCategoryTreeModel(
            logging::LogCategories& categories,
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

       private:
        [[nodiscard]] static logging::LogCategoryId _getIdFromIndex(
            const QModelIndex& idx
        );

        [[nodiscard]] logging::LogCategoryId _getChildAt(
            logging::LogCategoryId parent,
            int                    row
        ) const;

        [[nodiscard]] int _getRowOfChild(
            logging::LogCategoryId parent,
            logging::LogCategoryId child
        ) const;

        [[nodiscard]] logging::LogCategoryId _getParentId(
            logging::LogCategoryId id
        ) const;
    };

}   // namespace ui

#endif   // __UI__WIDGETS__LOGGING__DEBUG_SLOTS_TREE_VIEW_HPP__