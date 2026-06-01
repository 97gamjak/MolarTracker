#include "ui/transaction/stock_transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>
#include <mstd/enum.hpp>

#include "config/quantity.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"

namespace ui
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define COLUMN_LIST(X)  \
    X(Date)             \
    X(Type)             \
    X(Stock)            \
    X(Account)          \
    X(ReferenceAccount) \
    X(Quantity)         \
    X(Price)            \
    X(Fees)             \
    X(Description)

    MSTD_ENUM(StockTransactionColumn, std::uint8_t, COLUMN_LIST);

    namespace
    {
        /**
         * @brief Gets the label for a column.
         *
         * @param col The column to get the label for.
         * @return QString The label for the column.
         */
        QString getColLabel(StockTransactionColumn col)
        {
            if (col == StockTransactionColumn::ReferenceAccount)
                return "Reference Account";

            return QString::fromStdString(
                StockTransactionColumnMeta::toString(col)
            );
        }

        /**
         * @brief Gets the column from an index.
         *
         * @param index The index to get the column for.
         * @return StockTransactionColumn The column for the index.
         */
        StockTransactionColumn getColFromIndex(int index)
        {
            return static_cast<StockTransactionColumn>(index);
        }
    }   // namespace

    /**
     * @brief Construct a new Stock Transaction Table Model:: Stock Transaction
     * Table Model object
     *
     * @param parent
     */
    StockTransactionTableModel::StockTransactionTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    StockTransactionTableModel::~StockTransactionTableModel() = default;

    /**
     * @brief Sets the transactions for the model.
     *
     * @param transactions The transactions to set.
     * @param accountIdToName The mapping of account IDs to account names.
     */
    void StockTransactionTableModel::setTransactions(
        std::vector<drafts::StockTransactionOverview> transactions,
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
    int StockTransactionTableModel::rowCount(const QModelIndex& parent) const
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
    QVariant StockTransactionTableModel::data(
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
    QVariant StockTransactionTableModel::headerData(
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
    Qt::ItemFlags StockTransactionTableModel::flags(
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
    int StockTransactionTableModel::columnCount(const QModelIndex& parent) const
    {
        return parent.isValid() ? 0 : StockTransactionColumnMeta::size;
    }

    /**
     * @brief Gets the index of the description column.
     *
     * @return int The index of the description column.
     */
    int StockTransactionTableModel::getDescriptionIndex()
    {
        return static_cast<int>(StockTransactionColumn::Description);
    }

    /**
     * @brief Gets the index of the date column.
     *
     * @return int The index of the date column.
     */
    int StockTransactionTableModel::getDateIndex()
    {
        return static_cast<int>(StockTransactionColumn::Date);
    }

    /**
     * @brief display data for the table model
     *
     * @param transaction
     * @param col
     * @return QVariant
     */
    QVariant StockTransactionTableModel::_displayData(
        const drafts::StockTransactionOverview& transaction,
        int                                     col
    ) const
    {
        switch (getColFromIndex(col))
        {
            case StockTransactionColumn::Date:
            {
                return transaction.getTimestamp().toQDateTime().toString(
                    "yyyy-MM-dd"
                );
            }
            case StockTransactionColumn::Description:
                return QString::fromStdString(
                    transaction.getComment().value_or("")
                );
            case StockTransactionColumn::Type:
                return QString::fromStdString("");
            case StockTransactionColumn::Stock:
                return QString::fromStdString(transaction.getTicker());
            case StockTransactionColumn::Account:
            {
                const auto id = transaction.getSecurityAccount();

                if (!_accountIdToName.contains(id) || !id.isValid())
                    return "";

                return QString::fromStdString(_accountIdToName.at(id));
            }
            case StockTransactionColumn::ReferenceAccount:
            {
                const auto id = transaction.getCashAccount();
                if (!_accountIdToName.contains(id) || !id.isValid())
                    return "";

                return QString::fromStdString(_accountIdToName.at(id));
            }
            case StockTransactionColumn::Quantity:
                return QString::fromStdString(
                    transaction.getQuantity().toString()
                );
            case StockTransactionColumn::Price:
                return QString::fromStdString(
                    transaction.getUnitPrice().toString(2)
                );
            case StockTransactionColumn::Fees:
            {
                const auto& fees = transaction.getTotalFees();
                if (fees.isZero())
                    return QStringLiteral("-");

                return QString::fromStdString(fees.toString(2));
            }
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
    QVariant StockTransactionTableModel::_decorationData(
        const drafts::StockTransactionOverview& transaction,
        int                                     col
    )
    {
        if (getColFromIndex(col) != StockTransactionColumn::Quantity)
            return {};

        return transaction.getQuantity() > 0 ? QColor(Qt::GlobalColor::green)
                                             : QColor(Qt::GlobalColor::red);
    }
    /**
     * @brief text alignment data for the table model
     *
     * @param col
     * @return QVariant
     */
    QVariant StockTransactionTableModel::_textAlignmentData(int col)
    {
        if (getColFromIndex(col) == StockTransactionColumn::Quantity ||
            getColFromIndex(col) == StockTransactionColumn::Price)
            return {Qt::AlignRight | Qt::AlignVCenter};
        return {Qt::AlignLeft | Qt::AlignVCenter};
    }

    /**
     * @brief Gets the label for a column.
     *
     * @param col The column to get the label for.
     * @return QString The label for the column.
     */
    QString StockTransactionTableModel::_getColLabel(int col)
    {
        return getColLabel(static_cast<StockTransactionColumn>(col));
    }

}   // namespace ui