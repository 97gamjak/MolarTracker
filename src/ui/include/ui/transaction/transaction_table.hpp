#ifndef __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__
#define __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__

#include <qabstractitemmodel.h>

#include "config/id_types.hpp"
#include "config/strong_id.hpp"

namespace drafts
{
    class TransactionOverviewDraft;   // Forward declaration
}   // namespace drafts

namespace ui
{
    /**
     * @brief A base class for a model for displaying transaction data in a
     * table view
     *
     */
    class TransactionTableModel : public QAbstractTableModel
    {
        Q_OBJECT
       private:
        /// The list of transactions to display in the table
        std::vector<drafts::TransactionOverviewDraft> _transactions;

        /// A map of account IDs to account names for display purposes
        unorderedIdMap<AccountId, std::string> _accountIdToName;

       public:
        explicit TransactionTableModel(QObject* parent = nullptr);

        void setTransactions(
            std::vector<drafts::TransactionOverviewDraft> transactions,
            unorderedIdMap<AccountId, std::string>        accountIdToName
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

        /**
         * @brief Get the Description Index
         *
         * @return int
         */
        [[nodiscard]] virtual int getDescriptionIndex() const = 0;

        /**
         * @brief Get the Date Index
         *
         * @return int
         */
        [[nodiscard]] virtual int getDateIndex() const = 0;

       protected:
        /**
         * @brief display data for a specific column
         *
         * @param transaction
         * @param col
         * @return QVariant
         */
        [[nodiscard]]
        virtual QVariant _displayData(
            const drafts::TransactionOverviewDraft& transaction,
            int                                     col
        ) const = 0;

        /**
         * @brief Get the Decoration Data for a specific column
         *
         * @param transaction
         * @param col
         * @return QVariant
         */
        [[nodiscard]]
        virtual QVariant _decorationData(
            const drafts::TransactionOverviewDraft& transaction,
            int                                     col
        ) const = 0;

        /**
         * @brief Get the Text Alignment Data for a specific column
         *
         * @param col
         * @return QVariant
         */
        [[nodiscard]]
        virtual QVariant _textAlignmentData(int col) const = 0;

        /**
         * @brief Get the Column Label for a specific column
         *
         * @param col
         * @return QString
         */
        [[nodiscard]]
        virtual QString _getColLabel(int col) const = 0;

        [[nodiscard]]
        const unorderedIdMap<AccountId, std::string>& _getAccountIdToNameMap(
        ) const;
    };
}   // namespace ui

#endif   // __UI__INCLUDE__UI__TRANSACTION__TRANSACTION_TABLE_HPP__