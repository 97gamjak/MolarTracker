#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__

#include "finance/instrument/stock.hpp"
#include "utils/container/id_map.hpp"

namespace finance
{
    /**
     * @brief Class representing a collection of stocks, providing access to
     * individual stocks by their identifiers.
     *
     */
    class Stocks : public IdMap<Stock>
    {
       public:
        [[nodiscard]] std::unordered_set<std::string> getTickers() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__