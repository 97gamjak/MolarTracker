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
        using IdToNameMap =
            std::unordered_map<AccountId, std::string, AccountId::Hash>;

        std::vector<drafts::TransactionDraft> _transactions;

        IdToNameMap _accountIdToName;

       public:
        enum class Column : std::uint8_t
        {
            Date,
            Description,
            Type,
            FromAccount,
            ToAccount,
            Amount,
            Currency,
            Count
        };

        explicit TransactionTableModel(QObject* parent);

        void setTransactions(
            std::vector<drafts::TransactionDraft> transactions,
            IdToNameMap                           accountIdToName
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

}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__