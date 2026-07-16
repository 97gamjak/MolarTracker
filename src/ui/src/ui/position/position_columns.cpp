#include "position_columns.hpp"

#include <qcontainerfwd.h>

#include <QString>

namespace ui
{
    /**
     * @brief Convert a PositionColumn enum value to a QString.
     *
     * @param column The PositionColumn enum value.
     * @return QString The corresponding QString representation.
     */
    QString toQString(PositionColumns column)
    {
        using enum PositionColumns;

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

        const auto str = PositionColumnsMeta::toString(column);
        return QString::fromStdString(str);
    }
}   // namespace ui
