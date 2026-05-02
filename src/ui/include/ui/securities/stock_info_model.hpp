#ifndef __UI__INCLUDE__UI__SECURITIES__STOCK_INFO_MODEL_HPP__
#define __UI__INCLUDE__UI__SECURITIES__STOCK_INFO_MODEL_HPP__

#include <QAbstractTableModel>
#include <vector>

#include "StockInfoRow.hpp"

class StockInfoTableModel : public QAbstractTableModel
{
    Q_OBJECT

   public:
    enum class Column : int
    {
        Ticker = 0,
        Name,
        Price,
        Change,
        ChangePercent,
        PrevClose,
        Currency,
        LastUpdated,
        COUNT
    };

    explicit StockInfoTableModel(QObject* parent = nullptr);

    void setRows(std::vector<StockInfoRow> rows);
    void updateRow(const StockInfoRow& row);   // upsert by ticker

    int      rowCount(const QModelIndex& parent = {}) const override;
    int      columnCount(const QModelIndex& parent = {}) const override;
    QVariant data(
        const QModelIndex& index,
        int                role = Qt::DisplayRole
    ) const override;
    QVariant headerData(
        int             section,
        Qt::Orientation orientation,
        int             role = Qt::DisplayRole
    ) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

   private:
    std::vector<StockInfoRow> m_rows;

    static QString formatChange(double change, double pct);
    static QColor  changeColor(double change);
};