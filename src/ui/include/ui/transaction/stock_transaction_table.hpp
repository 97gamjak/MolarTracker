#ifndef __UI__INCLUDE__UI__TRANSACTION__STOCK_TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__STOCK_TRANSACTION_TABLE_HPP__

#include <qabstractitemmodel.h>
#include <qtablewidget.h>

namespace drafts
{
    class StockTransactionOverview;   // Forward declaration
}   // namespace drafts

namespace ui
{

    /**
     * @brief A draft representation of a stock transaction
     *
     */
    class StockTransactionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
       private:
        /// The list of transactions to display in the table
        std::vector<drafts::StockTransactionOverview> _transactions;

       public:
        explicit StockTransactionTableModel(QObject* parent = nullptr);
        ~StockTransactionTableModel() override;

        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;

        [[nodiscard]] static int getDescriptionIndex();
        [[nodiscard]] static int getDateIndex();

        void setTransactions(
            std::vector<drafts::StockTransactionOverview> transactions
        );

        [[nodiscard]]
        int rowCount(const QModelIndex& parent) const override;

        [[nodiscard]]
        QVariant data(const QModelIndex& index, int role) const override;

        [[nodiscard]]
        QVariant headerData(
            int             section,
            Qt::Orientation orientation,
            int             role
        ) const override;

        [[nodiscard]]
        Qt::ItemFlags flags(const QModelIndex& index) const override;

       private:
        [[nodiscard]]
        static QVariant _displayData(
            const drafts::StockTransactionOverview& transaction,
            int                                     col
        );

        [[nodiscard]]
        static QVariant _decorationData(
            const drafts::StockTransactionOverview& transaction,
            int                                     col
        );

        [[nodiscard]]
        static QVariant _textAlignmentData(int col);

        [[nodiscard]]
        static QString _getColLabel(int col);
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__STOCK_TRANSACTION_TABLE_HPP__