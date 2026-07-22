#include "finance/instrument/trade_filter_params.hpp"

namespace finance
{
    /**
     * @brief Set the symbol allowlist to restrict a securities query to
     *
     * @param symbolAllowlist The symbols to restrict the query to, nullopt
     * means no filtering
     */
    void TradeFilterParams::setSymbolAllowlist(
        std::optional<std::vector<std::string>> symbolAllowlist
    )
    {
        _symbolAllowlist = std::move(symbolAllowlist);
    }

    /**
     * @brief Get the symbol allowlist used to restrict a securities query
     *
     * @return const std::optional<std::vector<std::string>>&
     */
    const std::optional<std::vector<std::string>>&
    TradeFilterParams::getSymbolAllowlist() const
    {
        return _symbolAllowlist;
    }

}   // namespace finance
