#ifndef __UI__SRC__UI__UTILS__FORMAT_HPP__
#define __UI__SRC__UI__UTILS__FORMAT_HPP__

#include <cstdint>

#include "utils/finance.hpp"

class Cash;      // forward declaration
class QString;   // forward declaration

namespace ui
{
    [[nodiscard]]
    QString formatMicro(int64_t micro, Currency currency);

    [[nodiscard]]
    QString formatMicro(const Cash& cash);

    [[nodiscard]]
    QString formatBps(int64_t bps);
}   // namespace ui

#endif   // __UI__SRC__UI__UTILS__FORMAT_HPP__