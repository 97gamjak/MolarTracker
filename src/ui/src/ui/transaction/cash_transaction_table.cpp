#include "ui/transaction/cash_transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>
#include <mstd/enum.hpp>

#include "drafts/transaction/transaction_overview_draft.hpp"

namespace ui
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define COLUMN_LIST(X)  \
    X(Date)             \
    X(Type)             \
    X(Account)          \
    X(ReferenceAccount) \
    X(Amount)           \
    X(Fees)             \
    X(Description)

    MSTD_ENUM(CashTransactionColumn, std::uint8_t, COLUMN_LIST);

    namespace
    {
        /**
         * @brief Gets the label for a column.
         *
         * @param col The column to get the label for.
         * @return QString The label for the column.
         */
        QString getColLabel(CashTransactionColumn col)
        {
            if (col == CashTransactionColumn::ReferenceAccount)
                return "Reference Account";

            return QString::fromStdString(
                CashTransactionColumnMeta::toString(col)
            );
        }

        /**
         * @brief Gets the column from an index.
         *
         * @param index The index to get the column for.
         * @return CashTransactionColumn The column for the index.
         */
        CashTransactionColumn getColFromIndex(int index)
        {
            return static_cast<CashTransactionColumn>(index);
        }
    }   // namespace

    /**
     * @brief Construct a new Cash Transaction Table Model:: Cash Transaction
     * Table Model object
     *
     * @param parent
     */
    CashTransactionTableModel::CashTransactionTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    CashTransactionTableModel::~CashTransactionTableModel() = default;

    /**
     * @brief Sets the transactions for the model.
     *
     * @param transactions The transactions to set.
     * @param accountIdToName The mapping of account IDs to account names.
     */
    void CashTransactionTableModel::setTransactions(
        std::vector<drafts::CashTransactionOverview> transactions,
        IdMap<AccountId, std::string>                accountIdToName
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
    int CashTransactionTableModel::rowCount(const QModelIndex& parent) const
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
    QVariant CashTransactionTableModel::data(
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
     * @brief header data for the table model
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant CashTransactionTableModel::headerData(
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
    Qt::ItemFlags CashTransactionTableModel::flags(
        const QModelIndex& index
    ) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    /**
     * @brief get the number of columns in the table model
     *
     * @param parent
     * @return int
     */
    int CashTransactionTableModel::columnCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : CashTransactionColumnMeta::size;
    }

    /**
     * @brief Gets the index of the description column.
     *
     * @return int The index of the description column.
     */
    int CashTransactionTableModel::getDescriptionIndex()
    {
        return static_cast<int>(CashTransactionColumn::Description);
    }

    /**
     * @brief Gets the index of the date column.
     *
     * @return int The index of the date column.
     */
    int CashTransactionTableModel::getDateIndex()
    {
        return static_cast<int>(CashTransactionColumn::Date);
    }

    /**
     * @brief display data for the table model
     *
     * @param transaction
     * @param col
     * @return QVariant
     */
    QVariant CashTransactionTableModel::_displayData(
        const drafts::CashTransactionOverview& transaction,
        int                                    col
    ) const
    {
        switch (getColFromIndex(col))
        {
            case CashTransactionColumn::Date:
            {
                return transaction.getTimestamp().toQDateTime().toString(
                    "yyyy-MM-dd"
                );
            }
            case CashTransactionColumn::Description:
                return QString::fromStdString(
                    transaction.getComment().value_or("")
                );
            case CashTransactionColumn::Type:
                return QString::fromStdString("");
            case CashTransactionColumn::Account:
            {
                const auto id = transaction.getCashAccount();
                if (!_accountIdToName.contains(id))
                    return "";

                return QString::fromStdString(_accountIdToName.at(id));
            }
            case CashTransactionColumn::ReferenceAccount:
            {
                const auto id = transaction.getExternalAccount();
                if (!_accountIdToName.contains(id))
                    return "";

                return QString::fromStdString(_accountIdToName.at(id));
            }
            case CashTransactionColumn::Amount:
                return QString::fromStdString(
                    transaction.getAmount().toString(2)
                );
            case CashTransactionColumn::Fees:
                return QString::fromStdString(transaction.getFees().toString(2)
                );
        }
        return {};
    }

    /**
     * @brief decoration data for the table model
     *
     * @param transaction
     * @param col
     * @return QVariant
     */
    QVariant CashTransactionTableModel::_decorationData(
        const drafts::CashTransactionOverview& transaction,
        int                                    col
    )
    {
        if (getColFromIndex(col) != CashTransactionColumn::Amount)
            return {};

        return transaction.getAmount().isPositive()
                   ? QColor(Qt::GlobalColor::green)
                   : QColor(Qt::GlobalColor::red);
    }
    /**
     * @brief text alignment data for the table model
     *
     * @param col
     * @return QVariant
     */
    QVariant CashTransactionTableModel::_textAlignmentData(int col)
    {
        if (getColFromIndex(col) == CashTransactionColumn::Amount)
            return {Qt::AlignRight | Qt::AlignVCenter};
        return {Qt::AlignLeft | Qt::AlignVCenter};
    }

    /**
     * @brief Gets the label for a column.
     *
     * @param col The column to get the label for.
     * @return QString The label for the column.
     */
    QString CashTransactionTableModel::_getColLabel(int col)
    {
        return getColLabel(static_cast<CashTransactionColumn>(col));
    }

}   // namespace ui