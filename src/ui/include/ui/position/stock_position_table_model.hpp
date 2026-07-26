#ifndef __UI__INCLUDE__UI__POSITION__STOCK_POSITION_TABLE_MODEL_HPP__
#define __UI__INCLUDE__UI__POSITION__STOCK_POSITION_TABLE_MODEL_HPP__

#include <QAbstractTableModel>
#include <QString>
#include <vector>

#include "ui/position/position_columns.hpp"

namespace drafts
{
    class PositionStockDetailDraft;   // Forward declaration
}   // namespace drafts

namespace finance
{
    class PriceCache;   // Forward declaration
}   // namespace finance

namespace ui
{

    /**
     * @brief Model for displaying position data in a table view
     *
     */
    class StockPositionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
        /// Vector of position drafts
        std::vector<drafts::PositionStockDetailDraft> _positions;

       public:
        explicit StockPositionTableModel(QObject* parent = nullptr);

        [[nodiscard]]
        int rowCount(const QModelIndex& parent) const override;

        [[nodiscard]]
        int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]]
        QVariant data(const QModelIndex& index, int role) const override;

        [[nodiscard]]
        QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;

        void refreshPrices();

        void setPositions(
            const std::vector<drafts::PositionStockDetailDraft>& positions
        );

       private:
        [[nodiscard]]
        static QString _columnLabel(int index);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__POSITION__STOCK_POSITION_TABLE_MODEL_HPP__