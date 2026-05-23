#ifndef __UI__SRC__UI__POSITION__POSITION_COLUMNS_HPP__
#define __UI__SRC__UI__POSITION__POSITION_COLUMNS_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

class QString;

namespace ui
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define POSITION_COLUMNS_LIST(X) \
    X(Ticker)                    \
    X(Name)                      \
    X(OpenedAt)                  \
    X(Quantity)                  \
    X(AvgCost)                   \
    X(CostBasis)                 \
    X(LastPrice)                 \
    X(MarketValue)               \
    X(UnrealizedPnl)             \
    X(UnrealizedPnlPct)

    MSTD_ENUM(PositionColumns, std::uint8_t, POSITION_COLUMNS_LIST);

    [[nodiscard]]
    QString toQString(PositionColumns column);

}   // namespace ui

#endif   // __UI__SRC__UI__POSITION__POSITION_COLUMNS_HPP__