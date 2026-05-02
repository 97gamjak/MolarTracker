#include "ui/securities/stock_info_model.hpp"

#include <mstd/enum.hpp>

#include "config/finance.hpp"
#include "drafts/stock_draft.hpp"
#include "finance/ticker_info.hpp"

namespace ui
{
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STOCK_INFO_MODEL_COLUMNS(X) \
    X(Ticker)                       \
    X(Name)                         \
    X(Exchange)                     \
    X(Currency)                     \
    X(Industry)                     \
    X(Sector)                       \
    X(AssetClass)

    // cppcheck-suppress unknownMacro
    MSTD_ENUM(Column, std::uint8_t, STOCK_INFO_MODEL_COLUMNS)

    /**
     * @brief Construct a new Stock Info Table Model:: Stock Info Table Model
     * object
     *
     * @param parent
     */
    StockInfoTableModel::StockInfoTableModel(QObject* parent)
        : QAbstractTableModel(parent)
    {
    }

    /**
     * @brief Set the rows of the model
     *
     * @param rows The rows to set
     */
    void StockInfoTableModel::setRows(std::vector<drafts::StockInfoDraft> rows)
    {
        beginResetModel();
        _rows = std::move(rows);
        endResetModel();
    }

    /**
     * @brief Update a row in the model
     *
     * @param row The row to update
     */
    void StockInfoTableModel::updateRow(const drafts::StockInfoDraft& row)
    {
        for (size_t i = 0; i < _rows.size(); ++i)
        {
            if (_rows[i].getTicker() == row.getTicker())
            {
                _rows[i] = row;
                emit dataChanged(
                    index(static_cast<int>(i), 0),
                    index(static_cast<int>(i), columnCount({}) - 1),
                    {Qt::DisplayRole, Qt::ForegroundRole}
                );
                return;
            }
        }
        // Not found — append
        beginInsertRows(
            {},
            static_cast<int>(_rows.size()),
            static_cast<int>(_rows.size())
        );
        _rows.push_back(row);
        endInsertRows();
    }

    /**
     * @brief Get the number of rows in the model
     *
     * @param parent The parent index
     * @return int
     */
    int StockInfoTableModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(_rows.size());
    }

    /**
     * @brief Get the number of columns in the model
     *
     * @param parent The parent index
     * @return int
     */
    int StockInfoTableModel::columnCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;

        return static_cast<int>(ColumnMeta::size);
    }

    /**
     * @brief Get the data for a specific index
     *
     * @param index The index to get data for
     * @param role The role of the data
     * @return QVariant
     */
    QVariant StockInfoTableModel::data(const QModelIndex& index, int role) const
    {
        if (!index.isValid() || index.row() >= rowCount({}))
            return {};

        const auto& row = _rows[static_cast<size_t>(index.row())];
        const auto  col = static_cast<Column>(index.column());

        if (role == Qt::DisplayRole)
        {
            switch (col)
            {
                case Column::Ticker:
                    return QString::fromStdString(row.getTicker());
                case Column::Name:
                    return QString::fromStdString(row.getShortName());
                case Column::Exchange:
                    return QString::fromStdString(row.getExchange());
                case Column::Currency:
                    return QString::fromStdString(
                        CurrencyMeta::toString(row.getCurrency())
                    );
                case Column::Industry:
                    return QString::fromStdString(row.getIndustry());
                case Column::Sector:
                    return QString::fromStdString(row.getSector());
                case Column::AssetClass:
                    return QString::fromStdString(
                        finance::toString(row.getAssetClass())
                    );
            }

            std::unreachable();
        }

        if (role == Qt::TextAlignmentRole)
            return {Qt::AlignLeft | Qt::AlignVCenter};

        return {};
    }

    /**
     * @brief Get the header data for a specific section
     *
     * @param section The section to get header data for
     * @param orientation The orientation of the header
     * @param role The role of the header data
     * @return QVariant
     */
    QVariant StockInfoTableModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};

        return QString::fromStdString(
            ColumnMeta::toString(static_cast<Column>(section))
        );
    }

    /**
     * @brief Get the column index for the ticker
     *
     * @return int
     */
    int StockInfoTableModel::getTickerColumn()
    {
        return static_cast<int>(Column::Ticker);
    }

    /**
     * @brief flag handling for the model
     *
     * @param index
     * @return Qt::ItemFlags
     */
    Qt::ItemFlags StockInfoTableModel::flags(const QModelIndex& index) const
    {
        if (!index.isValid())
            return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

}   // namespace ui