#ifndef __UI__INCLUDE__UI__SECURITIES__STOCK_INFO_MODEL_HPP__
#define __UI__INCLUDE__UI__SECURITIES__STOCK_INFO_MODEL_HPP__

#include <QAbstractTableModel>
#include <vector>

#include "drafts/stock_draft.hpp"

namespace ui
{

    /**
     * @brief Model for displaying stock information in a table view.
     */
    class StockInfoTableModel : public QAbstractTableModel
    {
        Q_OBJECT

       private:
        /// The rows of the model
        std::vector<drafts::StockInfoDraft> _rows;

       public:
        explicit StockInfoTableModel(QObject* parent = nullptr);

        void setRows(std::vector<drafts::StockInfoDraft> rows);
        void updateRow(const drafts::StockInfoDraft& row);

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

        [[nodiscard]]
        static int getTickerColumn();

        [[nodiscard]]
        Qt::ItemFlags flags(const QModelIndex& index) const override;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__SECURITIES__STOCK_INFO_MODEL_HPP__