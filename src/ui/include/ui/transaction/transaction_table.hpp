#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__

#include <qtablewidget.h>

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>

#include "drafts/transaction_draft.hpp"

namespace ui
{
    class TransactionTableModel : public QAbstractTableModel
    {
        Q_OBJECT

       private:
        std::vector<drafts::TransactionDraft> _transactions;

       public:
        enum class Column : std::uint8_t
        {
            Date,
            Description,
            Type,
            Account,
            Amount,
            Currency,
            Count
        };

        explicit TransactionTableModel(QObject* parent);

        void setTransactions(
            std::vector<drafts::TransactionDraft> transactions
        );

        [[nodiscard]] int rowCount(const QModelIndex& parent) const override;
        [[nodiscard]] int columnCount(const QModelIndex& parent) const override;
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
        QVariant _displayData(
            const drafts::TransactionDraft& transaction,
            Column                          col
        ) const;
        [[nodiscard]]
        QVariant _decorationData(
            const drafts::TransactionDraft& transaction,
            Column                          col
        ) const;
        [[nodiscard]]
        QVariant _textAlignmentData(Column col) const;
    };

    class TransactionTable : public QWidget
    {
        Q_OBJECT

       private:
        TransactionTableModel* _model;
        QSortFilterProxyModel* _proxy;
        QTableView*            _table;

       public:
        explicit TransactionTable(QWidget* parent);

        TransactionTableModel* getModel();

       private:
        void _setupTable();
    };

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__