#ifndef __UI__INCLUDE__UI__POSITIONS__POSITION_TABLE_VIEW_HPP__
#define __UI__INCLUDE__UI__POSITIONS__POSITION_TABLE_VIEW_HPP__

#include <QTableView>

class QSortFilterProxyModel;   // Forward declaration

namespace ui
{
    class PositionTableModel;

    /**
     * @brief View for displaying position data in a table
     *
     */
    class PositionTableView : public QTableView
    {
        Q_OBJECT

       private:
        /// Pointer to the proxy model for filtering and sorting
        QSortFilterProxyModel* _proxy{nullptr};

       public:
        explicit PositionTableView(QWidget* parent = nullptr);

        void setPositionModel(PositionTableModel* model);

        /**
         * @brief Returns the source-model row for the current selection,
         * or -1 if nothing is selected.
         */
        [[nodiscard]] int selectedRow() const;

       private:
        void _setupColumns();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITIONS__POSITION_TABLE_VIEW_HPP__