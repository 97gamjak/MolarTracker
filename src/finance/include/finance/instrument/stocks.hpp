#ifndef __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__
#define __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__

#include "common/container/id_map.hpp"
#include "common/container/set.hpp"
#include "finance/instrument/stock.hpp"

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
        [[nodiscard]] Set<std::string> getTickers() const;
    };
}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__INSTRUMENT__STOCKS_HPP__