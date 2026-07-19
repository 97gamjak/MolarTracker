#include "format.hpp"

#include <QString>

#include "utils/cash.hpp"

namespace ui
{
    /**
     * @brief Formats a micro unit value into a QString with the specified
     * currency.
     *
     * @param micro The micro unit value to format.
     * @param currency The currency to display.
     * @return QString The formatted string.
     */
    QString formatMicro(int64_t micro, Currency currency)
    {
        return formatMicro(Cash{currency, micro});
    }

    /**
     * @brief Formats a cash object into a QString.
     *
     * @param cash The cash object to format.
     * @return QString The formatted string.
     */
    QString formatMicro(const Cash& cash)
    {
        return QString::fromStdString(cash.toString(2));
    }

    /**
     * @brief Formats basis points as a percentage string.
     *
     * @param bps The basis points value to format.
     * @return QString The formatted percentage string.
     */
    QString formatBps(int64_t bps)
    {
        const bool    negative   = bps < 0;
        const int64_t abs        = negative ? -bps : bps;
        const auto    percentage = 100;
        const auto    base       = 10;

        return QString{"%1%2.%3%"}
            .arg(negative ? "-" : "+")
            .arg(abs / percentage)
            .arg(abs % percentage, 2, base, QChar{'0'});
    }

}   // namespace ui
