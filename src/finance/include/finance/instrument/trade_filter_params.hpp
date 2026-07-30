#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__TRADE_FILTER_PARAMS_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__TRADE_FILTER_PARAMS_HPP__

#include <optional>
#include <string>
#include <vector>

namespace finance
{

    /**
     * @brief Filter parameters for querying securities, used to restrict a
     * securities query to a specific set of symbols (e.g. the symbols
     * contained in a watchlist). A `std::nullopt` allowlist means no symbol
     * filtering is applied.
     *
     */
    class TradeFilterParams
    {
       private:
        /// The symbols to restrict the query to, nullopt means no filtering
        std::optional<std::vector<std::string>> _symbolAllowlist;

       public:
        TradeFilterParams() = default;

        void setSymbolAllowlist(
            std::optional<std::vector<std::string>> symbolAllowlist
        );

        [[nodiscard]] const std::optional<std::vector<std::string>>&
        getSymbolAllowlist() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__TRADE_FILTER_PARAMS_HPP__
