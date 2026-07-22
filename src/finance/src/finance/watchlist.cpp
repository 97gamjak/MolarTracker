#include "finance/watchlist.hpp"

#include <format>

namespace finance
{
    /**
     * @brief Construct a new Watchlist:: Watchlist object
     *
     * @param name
     * @param createdAt
     * @param symbols
     */
    Watchlist::Watchlist(
        std::string               name,
        Timestamp                 createdAt,
        std::vector<std::string>  symbols
    )
        : _name(std::move(name)),
          _createdAt(createdAt),
          _symbols(std::move(symbols))
    {
    }

    /**
     * @brief Set the ID of the watchlist.
     *
     * @param id The new ID.
     */
    void Watchlist::setId(WatchlistId id) { _id = id; }

    /**
     * @brief Set the name of the watchlist.
     *
     * @param name The new name.
     */
    void Watchlist::setName(std::string name) { _name = std::move(name); }

    /**
     * @brief Set the symbols of the watchlist.
     *
     * @param symbols The new symbols.
     */
    void Watchlist::setSymbols(std::vector<std::string> symbols)
    {
        _symbols = std::move(symbols);
    }

    /**
     * @brief Get the ID of the watchlist.
     *
     * @return WatchlistId
     */
    WatchlistId Watchlist::getId() const { return _id; }

    /**
     * @brief Get the name of the watchlist.
     *
     * @return const std::string&
     */
    const std::string& Watchlist::getName() const { return _name; }

    /**
     * @brief Get the creation timestamp of the watchlist.
     *
     * @return Timestamp
     */
    Timestamp Watchlist::getCreatedAt() const { return _createdAt; }

    /**
     * @brief Get the symbols of the watchlist.
     *
     * @return const std::vector<std::string>&
     */
    const std::vector<std::string>& Watchlist::getSymbols() const
    {
        return _symbols;
    }

    /**
     * @brief Get a string representation of the watchlist.
     *
     * @return std::string
     */
    std::string Watchlist::toString() const
    {
        return std::format(
            "Watchlist(id={}, name={}, createdAt={}, symbols={})",
            _id.toString(),
            _name,
            _createdAt.humanReadable(),
            _symbols.size()
        );
    }

    /**
     * @brief Get a predicate for filtering watchlists by their ID
     *
     * @param id
     * @return filter::Predicate<Watchlist>
     */
    filter::Predicate<Watchlist> HasWatchlistId(WatchlistId id)
    {
        return filter::makePredicate<Watchlist>(
            [id](const Watchlist& watchlist)
            { return watchlist.getId() == id; }
        );
    }

}   // namespace finance
