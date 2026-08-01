#ifndef __FINANCE__INCLUDE__FINANCE__WATCHLIST_HPP__
#define __FINANCE__INCLUDE__FINANCE__WATCHLIST_HPP__

#include <string>

#include "common/container/set.hpp"
#include "common/timestamp.hpp"
#include "config/id_types.hpp"
#include "filter/predicate.hpp"

namespace finance
{

    /**
     * @brief Represents a named collection of symbols used to filter the
     * securities overview.
     *
     */
    class Watchlist
    {
       private:
        /// Watchlist ID
        WatchlistId _id;

        /// The display name of the watchlist
        std::string _name;

        /// Creation timestamp
        Timestamp _createdAt;

        /// The symbols contained in this watchlist, eagerly loaded
        Set<std::string> _symbols;

       public:
        explicit Watchlist(
            WatchlistId      id,
            std::string      name,
            Timestamp        createdAt,
            Set<std::string> symbols = {}
        );

        void setId(WatchlistId id);
        void setName(const std::string& name);

        [[nodiscard]] WatchlistId             getId() const;
        [[nodiscard]] const std::string&      getName() const;
        [[nodiscard]] Timestamp               getCreatedAt() const;
        [[nodiscard]] const Set<std::string>& getSymbols() const;

        [[nodiscard]]
        bool addSymbol(const std::string& symbol);

        [[nodiscard]]
        bool removeSymbol(const std::string& symbol);

        [[nodiscard]] std::string toString() const;
    };

    filter::Predicate<Watchlist> HasWatchlistId(WatchlistId id);

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__WATCHLIST_HPP__
