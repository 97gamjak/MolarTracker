#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__SECURITIES_FILTER_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__SECURITIES_FILTER_HPP__

#include <optional>
#include <string>

#include "common/container/set.hpp"
#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "finance/instrument/stock.hpp"

namespace finance
{

    /**
     * @brief Filter parameters for querying securities, used to restrict a
     * securities query to a specific set of symbols (e.g. the symbols
     * contained in a watchlist). A `std::nullopt` allowlist means no symbol
     * filtering is applied.
     *
     */
    struct SecuritiesFilter
    {
        /// The symbols to restrict the query to, nullopt means no filtering
        std::optional<Set<std::string>> symbols = std::nullopt;

        /// The instrument IDs corresponding to the symbols in the allowlist,
        std::optional<Set<StockId>> stockIds = std::nullopt;

        /// The instrument IDs corresponding to the symbols in the allowlist,
        std::optional<Set<InstrumentId>> instrumentIds = std::nullopt;

        SecuritiesFilter() = default;

        [[nodiscard]]
        filter::Predicate<Stock> getStockPredicate() const;
    };

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__SECURITIES_FILTER_HPP__
