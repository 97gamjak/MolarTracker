#ifndef __UI__SRC__UI__POSITION__POSITION_COLUMNS_HPP__
#define __UI__SRC__UI__POSITION__POSITION_COLUMNS_HPP__

#include <cstdint>
#include <mstd/enum.hpp>

class QString;

namespace ui
{

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define STOCK_POSITION_COLUMNS_LIST(X) \
    X(Ticker)                          \
    X(Name)                            \
    X(OpenedAt)                        \
    X(Quantity)                        \
    X(AvgCost)                         \
    X(CostBasis)                       \
    X(LastPrice)                       \
    X(MarketValue)                     \
    X(RealizedPnl)                     \
    X(RealizedPnlPct)                  \
    X(UnrealizedPnl)                   \
    X(UnrealizedPnlPct)

    MSTD_ENUM(StockPositionColumns, std::uint8_t, STOCK_POSITION_COLUMNS_LIST);

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define OPTION_POSITION_COLUMNS_LIST(X) \
    X(Ticker)                           \
    X(Name)                             \
    X(OpenedAt)                         \
    X(Quantity)                         \
    X(LastPrice)                        \
    X(MarketValue)                      \
    X(RealizedPnl)                      \
    X(RealizedPnlPct)                   \
    X(UnrealizedPnl)                    \
    X(UnrealizedPnlPct)

    MSTD_ENUM(
        OptionPositionColumns,
        std::uint8_t,
        OPTION_POSITION_COLUMNS_LIST
    );

    [[nodiscard]]
    QString toQString(StockPositionColumns column);

    [[nodiscard]]
    QString toQString(OptionPositionColumns column);

}   // namespace ui

#endif   // __UI__SRC__UI__POSITION__POSITION_COLUMNS_HPP__