#include "ui/position/position_columns.hpp"

#include <qcontainerfwd.h>

#include <QString>

namespace ui
{
    /**
     * @brief Convert a StockPositionColumn enum value to a QString.
     *
     * @param column The StockPositionColumn enum value.
     * @return QString The corresponding QString representation.
     */
    QString toQString(StockPositionColumns column)
    {
        using enum StockPositionColumns;

        if (column == OpenedAt)
            return "Open Date";

        if (column == AvgCost)
            return "Average Cost";

        if (column == CostBasis)
            return "Cost Basis";

        if (column == LastPrice)
            return "Last Price";

        if (column == MarketValue)
            return "Market Value";

        if (column == UnrealizedPnl)
            return "Unrealized P&L";

        if (column == UnrealizedPnlPct)
            return "Unrealized P&L %";

        const auto str = StockPositionColumnsMeta::toString(column);
        return QString::fromStdString(str);
    }

    /**
     * @brief Convert a OptionPositionColumn enum value to a QString.
     *
     * @param column The OptionPositionColumn enum value.
     * @return QString The corresponding QString representation.
     */
    QString toQString(OptionPositionColumns column)
    {
        using enum OptionPositionColumns;

        if (column == OpenedAt)
            return "Open Date";

        // if (column == AvgCost)
        //     return "Average Cost";

        // if (column == CostBasis)
        //     return "Cost Basis";

        if (column == LastPrice)
            return "Last Price";

        if (column == MarketValue)
            return "Market Value";

        if (column == UnrealizedPnl)
            return "Unrealized P&L";

        if (column == UnrealizedPnlPct)
            return "Unrealized P&L %";

        const auto str = OptionPositionColumnsMeta::toString(column);
        return QString::fromStdString(str);
    }
}   // namespace ui
