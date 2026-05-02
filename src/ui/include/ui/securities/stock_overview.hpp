#ifndef __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__

#include <QWidget>

class QTableView;
class QSortFilterProxyModel;

namespace ui
{
    class StockInfoTableModel;

    /**
     * @brief Widget for displaying stock overview information.
     */
    class StockOverviewWidget : public QWidget
    {
        Q_OBJECT

       private:
        /// The model for displaying stock information
        StockInfoTableModel* _model;
        /// The proxy model for filtering and sorting
        QSortFilterProxyModel* _proxy;
        /// The table view for displaying stock information
        QTableView* _table;

       public:
        explicit StockOverviewWidget(QWidget* parent = nullptr);

        [[nodiscard]]
        StockInfoTableModel* getModel() const;

       signals:
        /**
         * @brief Emitted when a stock ticker is selected.
         *
         * @param ticker The selected stock ticker.
         */
        void tickerSelected(const QString& ticker);

       private:
        void _setupTable();
        void _onSelectionChanged();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__
