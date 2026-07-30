#include "ui/transaction/option_transaction_table.hpp"

#include <qboxlayout.h>
#include <qheaderview.h>
#include <qlineedit.h>
#include <qnamespace.h>
#include <qvariant.h>

#include <QColor>
#include <QDateTime>
#include <QFont>
#include <mstd/enum.hpp>

#include "common/finance.hpp"
#include "common/quantity.hpp"
#include "drafts/transaction/transaction_overview_draft.hpp"

namespace ui
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define COLUMN_LIST(X)  \
    X(Date)             \
    X(Type)             \
    X(Option)           \
    X(Account)          \
    X(ReferenceAccount) \
    X(BuySell)          \
    X(Action)           \
    X(Quantity)         \
    X(Premium)          \
    X(Fees)             \
    X(Description)

    MSTD_ENUM(OptionTransactionColumn, std::uint8_t, COLUMN_LIST);

    namespace
    {
        /**
         * @brief Gets the label for a column.
         *
         * @param col The column to get the label for.
         * @return QString The label for the column.
         */
        QString getColLabel(OptionTransactionColumn col)
        {
            if (col == OptionTransactionColumn::ReferenceAccount)
                return "Reference Account";
            if (col == OptionTransactionColumn::BuySell)
                return "Buy/Sell";

            return QString::fromStdString(
                OptionTransactionColumnMeta::toString(col)
            );
        }

        /**
         * @brief Gets the column from an index.
         *
         * @param index The index to get the column for.
         * @return OptionTransactionColumn The column for the index.
         */
        OptionTransactionColumn getColFromIndex(int index)
        {
            return static_cast<OptionTransactionColumn>(index);
        }
    }   // namespace

    /**
     * @brief Construct a new Option Transaction Table Model:: Option
     * Transaction Table Model object
     *
     * @param parent
     */
    OptionTransactionTableModel::OptionTransactionTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    OptionTransactionTableModel::~OptionTransactionTableModel() = default;

    /**
     * @brief Sets the transactions for the model.
     *
     * @param transactions The transactions to set.
     * @param accountIdToName The mapping of account IDs to account names.
     */
    void OptionTransactionTableModel::setTransactions(
        std::vector<drafts::OptionTransactionOverview> transactions,
        IdMap<AccountId, std::string>                  accountIdToName
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
    int OptionTransactionTableModel::rowCount(const QModelIndex& parent) const
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
    QVariant OptionTransactionTableModel::data(
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
    QVariant OptionTransactionTableModel::headerData(
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
    Qt::ItemFlags OptionTransactionTableModel::flags(
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
    int OptionTransactionTableModel::columnCount(
        const QModelIndex& parent
    ) const
    {
        return parent.isValid() ? 0 : OptionTransactionColumnMeta::size;
    }

    /**
     * @brief Gets the index of the description column.
     *
     * @return int The index of the description column.
     */
    int OptionTransactionTableModel::getDescriptionIndex()
    {
        return static_cast<int>(OptionTransactionColumn::Description);
    }

    /**
     * @brief Gets the index of the date column.
     *
     * @return int The index of the date column.
     */
    int OptionTransactionTableModel::getDateIndex()
    {
        return static_cast<int>(OptionTransactionColumn::Date);
    }

    /**
     * @brief display data for the table model
     *
     * @param transaction
     * @param col
     * @return QVariant
     */
    QVariant OptionTransactionTableModel::_displayData(
        const drafts::OptionTransactionOverview& transaction,
        int                                      col
    ) const
    {
        switch (getColFromIndex(col))
        {
            case OptionTransactionColumn::Date:
            {
                return transaction.getTimestamp().toQDateTime().toString(
                    "yyyy-MM-dd"
                );
            }
            case OptionTransactionColumn::Description:
                return QString::fromStdString(
                    transaction.getComment().value_or("")
                );
            case OptionTransactionColumn::Type:
                return QString::fromStdString("");
            case OptionTransactionColumn::Option:
                return QString::fromStdString(transaction.getOptionName());
            case OptionTransactionColumn::Account:
            {
                const auto id = transaction.getSecurityAccount();

                if (!_accountIdToName.contains(id) || !id.isValid())
                    return "";

                return QString::fromStdString(_accountIdToName.at(id));
            }
            case OptionTransactionColumn::ReferenceAccount:
            {
                const auto id = transaction.getCashAccount();
                if (!_accountIdToName.contains(id) || !id.isValid())
                    return "";

                return QString::fromStdString(_accountIdToName.at(id));
            }
            case OptionTransactionColumn::BuySell:
                return QString::fromStdString(
                    OptionBuySellMeta::toString(transaction.getBuySell())
                );
            case OptionTransactionColumn::Action:
                return QString::fromStdString(
                    TransactionOptionActionMeta::toString(
                        transaction.getAction()
                    )
                );
            case OptionTransactionColumn::Quantity:
                return QString::fromStdString(
                    transaction.getQuantity().toString()
                );
            case OptionTransactionColumn::Premium:
                return QString::fromStdString(
                    transaction.getPremium().toString(2)
                );
            case OptionTransactionColumn::Fees:
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
    QVariant OptionTransactionTableModel::_decorationData(
        const drafts::OptionTransactionOverview& transaction,
        int                                      col
    )
    {
        if (getColFromIndex(col) != OptionTransactionColumn::Quantity)
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
    QVariant OptionTransactionTableModel::_textAlignmentData(int col)
    {
        if (getColFromIndex(col) == OptionTransactionColumn::Quantity ||
            getColFromIndex(col) == OptionTransactionColumn::Premium)
            return {Qt::AlignRight | Qt::AlignVCenter};
        return {Qt::AlignLeft | Qt::AlignVCenter};
    }

    /**
     * @brief Gets the label for a column.
     *
     * @param col The column to get the label for.
     * @return QString The label for the column.
     */
    QString OptionTransactionTableModel::_getColLabel(int col)
    {
        return getColLabel(static_cast<OptionTransactionColumn>(col));
    }

}   // namespace ui
