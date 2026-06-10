#ifndef __UI__SRC__UI__UTILS__FORMAT_HPP__
#define __UI__SRC__UI__UTILS__FORMAT_HPP__

#include <cstdint>

#include "config/finance.hpp"

namespace finance
{
    class Cash;   // forward declaration
}   // namespace finance

class QString;

namespace ui
{
    [[nodiscard]]
    QString formatMicro(int64_t micro, Currency currency);

    [[nodiscard]]
    QString formatMicro(const finance::Cash& cash);

    [[nodiscard]]
    QString formatBps(int64_t bps);
}   // namespace ui

#endif   // __UI__SRC__UI__UTILS__FORMAT_HPP__