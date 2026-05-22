#include "ui/transaction/transaction_table.hpp"

#include "drafts/transaction_draft.hpp"

namespace ui
{
    /**
     * @brief Construct a new Transaction Table Model:: Transaction Table Model
     * object
     *
     * @param parent
     */
    TransactionTableModel::TransactionTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    /**
     * @brief Sets the transactions for the model.
     *
     * @param transactions The transactions to set.
     * @param accountIdToName The mapping of account IDs to account names.
     */
    void TransactionTableModel::setTransactions(
        std::vector<drafts::TransactionOverviewDraft> transactions,
        unorderedIdMap<AccountId, std::string>        accountIdToName
    )
    {
        beginResetModel();
        _transactions    = std::move(transactions);
        _accountIdToName = std::move(accountIdToName);
        endResetModel();
    }

    /**
     * @brief get the number of rows in the table model
     *
     * @param parent
     * @return int
     */
    int TransactionTableModel::rowCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : static_cast<int>(_transactions.size());
    }

    /**
     * @brief data method for the table model
     *
     * @param index
     * @param role
     * @return QVariant
     */
    QVariant TransactionTableModel::data(
        const QModelIndex& index,
        int                role
    ) const
    {
        if (!index.isValid() || index.row() >= rowCount({}))
            return {};

        const auto& transaction =
            _transactions[static_cast<std::size_t>(index.row())];

        switch (role)
        {
            case Qt::DisplayRole:
                return _displayData(transaction, index.column());
            case Qt::DecorationRole:
                return _decorationData(transaction, index.column());
            case Qt::TextAlignmentRole:
                return _textAlignmentData(index.column());
            default:
                return {};
        }
    }

    /**
     * @brief Gets the mapping of account IDs to account names.
     *
     * @return const TransactionTableModel::IdToNameMap& The mapping of account
     * IDs to account names.
     */
    const unorderedIdMap<AccountId, std::string>& TransactionTableModel::
        _getAccountIdToNameMap() const
    {
        return _accountIdToName;
    }

    /**
     * @brief header data for the table model
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant TransactionTableModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};

        return _getColLabel(section);
    }

    /**
     * @brief the item flags for the table model
     *
     * @param index
     * @return Qt::ItemFlags
     */
    Qt::ItemFlags TransactionTableModel::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
}   // namespace ui