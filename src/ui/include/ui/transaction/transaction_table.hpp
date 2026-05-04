#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__

#include <qabstractitemmodel.h>

#include "config/id_types.hpp"

namespace drafts
{
    class TransactionOverviewDraft;
}

namespace ui
{
    class TransactionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
       private:
        /// alias for the map of account IDs to account names
        using IdToNameMap =
            std::unordered_map<AccountId, std::string, AccountId::Hash>;

        /// The list of transactions to display in the table
        std::vector<drafts::TransactionOverviewDraft> _transactions;

        /// A map of account IDs to account names for display purposes
        IdToNameMap _accountIdToName;

       public:
        explicit TransactionTableModel(QObject* parent = nullptr);

        void setTransactions(
            std::vector<drafts::TransactionOverviewDraft> transactions,
            IdToNameMap                                   accountIdToName
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

        [[nodiscard]] virtual int getDescriptionIndex() const = 0;
        [[nodiscard]] virtual int getDateIndex() const        = 0;

       protected:
        [[nodiscard]]
        virtual QVariant _displayData(
            const drafts::TransactionOverviewDraft& transaction,
            int                                     col
        ) const = 0;
        [[nodiscard]]
        virtual QVariant _decorationData(
            const drafts::TransactionOverviewDraft& transaction,
            int                                     col
        ) const = 0;
        [[nodiscard]]
        virtual QVariant _textAlignmentData(int col) const = 0;

        [[nodiscard]]
        virtual QString _getColLabel(int col) const = 0;

        const IdToNameMap& _getAccountIdToNameMap() const;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__