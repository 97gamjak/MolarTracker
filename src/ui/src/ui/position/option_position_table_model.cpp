#include "ui/position/option_position_table_model.hpp"

#include <QColor>
#include <QDateTime>

#include "drafts/position/position_option_draft.hpp"
#include "ui/position/position_columns.hpp"
#include "ui/utils/format.hpp"

namespace ui
{
    namespace
    {
        /**
         * @brief Display a price as a string, formatted with two decimal
         * places.
         *
         * @param price The price to display.
         * @return QString The formatted price string.
         */
        QString displayPrice(const Cash& price)
        {
            if (price.getCurrency() == Currency::Unknown || price.isZero())
                return "-";

            return QString::fromStdString(price.toString(2));
        }

        /**
         * @brief Display a percentage as a string, formatted with two decimal
         * places and a percent sign, with a "+" sign for positive values.
         *
         * @param percentage The percentage to display.
         * @return QString The formatted percentage string.
         */
        QString displayPercentage(Percentage percentage)
        {
            return QString::fromStdString(percentage.toString());
        }
    }   // namespace

    /**
     * @brief Construct a new Position Table Model:: Position Table Model object
     *
     * @param parent The parent QObject (optional).
     */
    OptionPositionTableModel::OptionPositionTableModel(QObject* parent)
        : QAbstractTableModel{parent}
    {
    }

    /**
     * @brief Get the number of rows in the model.
     *
     * @param parent The parent index (unused).
     * @return int The number of rows.
     */
    int OptionPositionTableModel::rowCount(const QModelIndex& parent) const
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
    int OptionPositionTableModel::columnCount(const QModelIndex& parent) const
    {
        if (parent.isValid())
            return 0;
        return static_cast<int>(OptionPositionColumnsMeta::size);
    }

    /**
     * @brief Get the data for a specific index and role.
     *
     * @param index The model index.
     * @param role The role for which to retrieve data.
     * @return QVariant The data for the specified index and role.
     */
    QVariant OptionPositionTableModel::data(
        const QModelIndex& index,
        int                role
    ) const
    {
        if (!index.isValid() || index.row() >= rowCount({}))
            return {};

        const auto& pos = _positions[static_cast<std::size_t>(index.row())];
        const auto  col = static_cast<OptionPositionColumns>(index.column());

        switch (role)
        {
            case Qt::DisplayRole:
            {
                switch (col)
                {
                    case OptionPositionColumns::Ticker:
                        return QString::fromStdString(
                            pos.getStockInfo().getTicker()
                        );

                    case OptionPositionColumns::Name:
                        return QString::fromStdString(
                            pos.getStockInfo().getShortName()
                        );

                    case OptionPositionColumns::OpenedAt:
                        return pos.getCreatedAt().toQDateTime().toString(
                            "yyyy-MM-dd"
                        );

                    case OptionPositionColumns::Quantity:
                        return QString::fromStdString(
                            pos.getQuantity().toString()
                        );
                        // case OptionPositionColumns::AvgCost:
                        //     return displayPrice(pos.getAveragePrice());
                        // case OptionPositionColumns::CostBasis:
                        //     return displayPrice(pos.getTotalPrice());

                    case OptionPositionColumns::MarketValue:
                        return displayPrice(pos.getMarketValue());
                    case OptionPositionColumns::UnrealizedPnl:
                        return displayPrice(pos.getUnrealizedPnL());
                    case OptionPositionColumns::UnrealizedPnlPct:
                        return displayPercentage(pos.getUnrealizedPnLPercentage(
                        ));
                    case OptionPositionColumns::RealizedPnl:
                        return displayPrice(pos.getRealizedPnL());
                    case OptionPositionColumns::RealizedPnlPct:
                        return displayPercentage(pos.getRealizedPnLPercentage()
                        );
                    case OptionPositionColumns::LastPrice:
                        return displayPrice(pos.getCurrentPrice());
                }
                std::unreachable();
            }

            case Qt::TextAlignmentRole:
            {
                switch (col)
                {
                    case OptionPositionColumns::Ticker:
                    case OptionPositionColumns::Name:
                    case OptionPositionColumns::OpenedAt:
                        return static_cast<int>(
                            Qt::AlignLeft | Qt::AlignVCenter
                        );

                    case OptionPositionColumns::Quantity:
                    // case OptionPositionColumns::AvgCost:
                    // case OptionPositionColumns::CostBasis:
                    case OptionPositionColumns::LastPrice:
                    case OptionPositionColumns::MarketValue:
                    case OptionPositionColumns::UnrealizedPnl:
                    case OptionPositionColumns::UnrealizedPnlPct:
                    case OptionPositionColumns::RealizedPnl:
                    case OptionPositionColumns::RealizedPnlPct:
                        return static_cast<int>(
                            Qt::AlignRight | Qt::AlignVCenter
                        );
                }
                std::unreachable();
            }

            default:
                return {};
        }

        return {};
    }

    /**
     * @brief Get the header data for a specific section, orientation, and role.
     *
     * @param section The section index.
     * @param orientation The orientation (horizontal or vertical).
     * @param role The role for which to retrieve header data.
     * @return QVariant The header data for the specified section, orientation,
     * and role.
     */
    QVariant OptionPositionTableModel::headerData(
        int             section,
        Qt::Orientation orientation,
        int             role
    ) const
    {
        if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
            return {};
        return _columnLabel(section);
    }

    /**
     * @brief Refresh the prices displayed in the model, this will emit data
     * changed signals for all price-related columns to ensure that the latest
     * prices are displayed in the view.
     */
    void OptionPositionTableModel::refreshPrices()
    {
        const int first = static_cast<int>(OptionPositionColumns::LastPrice);
        const int last =
            static_cast<int>(OptionPositionColumns::UnrealizedPnlPct);
        const int rows = rowCount({}) - 1;

        if (rows < 0)
            return;

        emit dataChanged(
            index(0, first),
            index(rows, last),
            {Qt::DisplayRole, Qt::ForegroundRole}
        );
    }

    /**
     * @brief Set the positions to be displayed in the model, this will reset
     * the model and update the internal list of position drafts to be
     * displayed in the view.
     *
     * @param positions The list of position drafts to display.
     */
    void OptionPositionTableModel::setPositions(
        const std::vector<drafts::PositionOptionDetailDraft>& positions
    )
    {
        beginResetModel();
        _positions = positions;
        endResetModel();
    }

    /**
     * @brief Get the label for a specific column index, this will return a
     * user-friendly label for the specified column index to be displayed in the
     * header of the table view.
     *
     * @param index The column index.
     * @return QString The label for the specified column index.
     */
    QString OptionPositionTableModel::_columnLabel(int index)
    {
        const auto column = static_cast<OptionPositionColumns>(index);
        return toQString(column);
    }

}   // namespace ui