#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__

#include <memory>

#include "finance/instrument/stock.hpp"
#include "utils/container/id_map.hpp"
#include "utils/container/set.hpp"

namespace finance
{
    /**
     * @brief Class representing a collection of stocks, providing access to
     * individual stocks by their identifiers.
     *
     */
    class Stocks : public IdObjectMap<Stock>
    {
       public:
        using IdObjectMap<Stock>::IdObjectMap;

        [[nodiscard]] IdSet<InstrumentId> getInstrumentIds() const;
    };

    /**
     * @brief Class representing a view of stocks, providing access to
     * individual stocks by their identifiers.
     *
     */
    class StocksView : public IdObjectMap<std::shared_ptr<const Stock>>
    {
       public:
        using IdObjectMap<std::shared_ptr<const Stock>>::IdObjectMap;

        [[nodiscard]] std::unordered_set<std::string> getTickers() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__