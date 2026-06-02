#include "ui/position/position_table_model.hpp"

#include <QColor>
#include <QDateTime>

#include "drafts/position_draft.hpp"
#include "finance/price_cache.hpp"
#include "ui/position/position_columns.hpp"
#include "ui/utils/format.hpp"

namespace ui
{

    /**
     * @brief Construct a new Position Table Model:: Position Table Model object
     *
     * @param priceCache
     * @param parent
     */
    StockPositionTableModel::StockPositionTableModel(
        const finance::PriceCache* priceCache,
        QObject*                   parent
    )
        : QAbstractTableModel{parent}, _priceCache{priceCache}
    {
    }

    /**
     * @brief Get the number of rows in the model.
     *
     * @param parent The parent index (unused).
     * @return int The number of rows.
     */
    int StockPositionTableModel::rowCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(_positions.size());
    }

    /**
     * @brief Get the number of columns in the model.
     *
     * @param parent The parent index (unused).
     * @return int The number of columns.
     */
    int StockPositionTableModel::columnCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(PositionColumnsMeta::size);
    }

    /**
     * @brief Get the data for a specific index and role.
     *
     * @param index The model index.
     * @param role The role for which to retrieve data.
     * @return QVariant The data for the specified index and role.
     */
    QVariant StockPositionTableModel::data(
        const QModelIndex& index,
        int                role
    ) const
    {
        if (!index.isValid() || index.row() >= rowCount({}))
            return {};

        const auto& pos = _positions[static_cast<std::size_t>(index.row())];
        const auto  col = static_cast<PositionColumns>(index.column());

        switch (role)
        {
            case Qt::DisplayRole:
            {
                switch (col)
                {
                    case PositionColumns::Ticker:
                        return QString::fromStdString(
                            pos.getStockInfo().getTicker()
                        );

                    case PositionColumns::Name:
                        return QString::fromStdString(
                            pos.getStockInfo().getShortName()
                        );

                    case PositionColumns::OpenedAt:
                        return pos.getCreatedAt().toQDateTime().toString(
                            "yyyy-MM-dd"
                        );

                    // TODO(97gamjak): wire up once PositionStockDetailDraft (or
                    // PositionSummary) carries aggregated trade-leg fields.
                    case PositionColumns::Quantity:
                        return QString::fromStdString(
                            pos.getQuantity().toString()
                        );
                    case PositionColumns::AvgCost:
                    case PositionColumns::CostBasis:
                        return tr("—");

                    case PositionColumns::LastPrice:
                    case PositionColumns::MarketValue:
                    case PositionColumns::UnrealizedPnl:
                    case PositionColumns::UnrealizedPnlPct:
                        return _priceDisplay(pos, col);
                }
                std::unreachable();
            }

            case Qt::TextAlignmentRole:
            {
                switch (col)
                {
                    case PositionColumns::Ticker:
                    case PositionColumns::Name:
                    case PositionColumns::OpenedAt:
                        return static_cast<int>(
                            Qt::AlignLeft | Qt::AlignVCenter
                        );

                    case PositionColumns::Quantity:
                    case PositionColumns::AvgCost:
                    case PositionColumns::CostBasis:
                    case PositionColumns::LastPrice:
                    case PositionColumns::MarketValue:
                    case PositionColumns::UnrealizedPnl:
                    case PositionColumns::UnrealizedPnlPct:
                        return static_cast<int>(
                            Qt::AlignRight | Qt::AlignVCenter
                        );
                }
                std::unreachable();
            }

            case Qt::ForegroundRole:
            {
                // Colour unrealized P&L green/red once values are available.
                if (col != PositionColumns::UnrealizedPnl &&
                    col != PositionColumns::UnrealizedPnlPct)
                    return {};

                // TODO: use PositionSummary::yahooSymbol once available.
                if (_priceCache == nullptr)
                    return {};
                const auto quote =
                    _priceCache->get(pos.getStockInfo().getTicker());
                if (!quote)
                    return {};

                // Placeholder until avgCostMicroUnits() is on the domain type.
                // Replace the 0 with pos.avgCostMicroUnits().
                const int64_t avgCost = 0;
                if (quote->getPrice().getAmount() >= avgCost)
                    return QColor{Qt::green};
                return QColor{Qt::red};
            }

            default:
                return {};
        }
    }

    QVariant StockPositionTableModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        return _columnLabel(section);
    }

    void StockPositionTableModel::refreshPrices()
    {
        const int first = static_cast<int>(PositionColumns::LastPrice);
        const int last  = static_cast<int>(PositionColumns::UnrealizedPnlPct);
        const int rows  = rowCount({}) - 1;

        if (rows < 0)
            return;

        emit dataChanged(
            index(0, first),
            index(rows, last),
            {Qt::DisplayRole, Qt::ForegroundRole}
        );
    }

    void StockPositionTableModel::setPositions(
        const std::vector<drafts::PositionStockDetailDraft>& positions
    )
    {
        beginResetModel();
        _positions = positions;
        endResetModel();
    }

    // ---------------------------------------------------------------------------
    // Private helpers
    // ---------------------------------------------------------------------------

    QVariant StockPositionTableModel::_priceDisplay(
        const drafts::PositionStockDetailDraft& pos,
        PositionColumns                         col
    ) const
    {
        // TODO: replace getTicker() with yahooSymbol once available on the
        // type.
        if (_priceCache == nullptr)
            return {};

        const auto quote = _priceCache->get(pos.getStockInfo().getTicker());
        if (!quote)
            return tr("—");

        switch (col)
        {
            case PositionColumns::LastPrice:
                return formatMicro(quote->getPrice());

            // TODO: the three cases below require netQuantity and
            // avgCostMicroUnits from the domain type. Stubbed to "—" until
            // PositionSummary exists.
            case PositionColumns::AvgCost:
            case PositionColumns::MarketValue:
            case PositionColumns::UnrealizedPnl:
            case PositionColumns::UnrealizedPnlPct:
            case PositionColumns::CostBasis:
                return tr("—");

            case PositionColumns::Ticker:
            case PositionColumns::Name:
            case PositionColumns::OpenedAt:
            case ui::PositionColumns::Quantity:
                return {};
        }

        std::unreachable();
    }

    QString StockPositionTableModel::_columnLabel(int index)
    {
        const auto column = static_cast<PositionColumns>(index);
        return toQString(column);
    }

}   // namespace ui