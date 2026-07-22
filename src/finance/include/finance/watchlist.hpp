#ifndef __FINANCE__INCLUDE__FINANCE__WATCHLIST_HPP__
#define __FINANCE__INCLUDE__FINANCE__WATCHLIST_HPP__

#include <string>
#include <vector>

#include "config/id_types.hpp"
#include "filter/predicate.hpp"
#include "utils/timestamp.hpp"

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
        std::vector<std::string> _symbols;

       public:
        explicit Watchlist(
            std::string               name,
            Timestamp                 createdAt,
            std::vector<std::string>  symbols = {}
        );

        void setId(WatchlistId id);
        void setName(std::string name);
        void setSymbols(std::vector<std::string> symbols);

        [[nodiscard]] WatchlistId                getId() const;
        [[nodiscard]] const std::string&          getName() const;
        [[nodiscard]] Timestamp                   getCreatedAt() const;
        [[nodiscard]] const std::vector<std::string>& getSymbols() const;

        [[nodiscard]] std::string toString() const;
    };

    filter::Predicate<Watchlist> HasWatchlistId(WatchlistId id);

}   // namespace finance

#endif   // __FINANCE__INCLUDE__FINANCE__WATCHLIST_HPP__
