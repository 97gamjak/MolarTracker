#ifndef __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__
#define __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__

#include <QWidget>

class QLineEdit;
class QTableView;
class QSortFilterProxyModel;
class StockInfoTableModel;
struct StockInfoRow;

namespace ui
{

    class StockOverviewWidget : public QWidget
    {
        Q_OBJECT

       private:
        StockInfoTableModel*   _model;
        QSortFilterProxyModel* _proxy;
        QTableView*            _table;

       public:
        explicit StockOverviewWidget(QWidget* parent = nullptr);

        [[nodiscard]]
        StockInfoTableModel* getModel() const;

       signals:
        void tickerSelected(const QString& ticker);

       private:
        void _setupTable();
        void _onSelectionChanged();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__SECURITIES__STOCK_OVERVIEW_HPP__
