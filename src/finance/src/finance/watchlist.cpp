#include "finance/watchlist.hpp"

#include <format>

namespace finance
{
    /**
     * @brief Construct a new Watchlist:: Watchlist object
     *
     * @param id
     * @param name
     * @param createdAt
     * @param symbols
     */
    Watchlist::Watchlist(
        WatchlistId      id,
        std::string      name,
        Timestamp        createdAt,
        Set<std::string> symbols
    )
        : _id(id),
          _name(std::move(name)),
          _createdAt(createdAt),
          _symbols(std::move(symbols))
    {
    }

    /**
     * @brief Set the ID of the watchlist.
     *
     * @param id
     */
    void Watchlist::setId(WatchlistId id) { _id = id; }

    /**
     * @brief Set the name of the watchlist.
     *
     * @param name
     */
    void Watchlist::setName(const std::string& name) { _name = name; }

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
     * @return const Set<std::string>&
     */
    const Set<std::string>& Watchlist::getSymbols() const { return _symbols; }

    /**
     * @brief Add a symbol to the watchlist.
     *
     * @param symbol
     * @return true if the symbol was added, false if it already exists
     */
    bool Watchlist::addSymbol(const std::string& symbol)
    {
        if (_symbols.contains(symbol))
            return false;

        _symbols.insert(symbol);
        return true;
    }

    /**
     * @brief Remove a symbol from the watchlist.
     *
     * @param symbol
     * @return true if the symbol was removed, false if it did not exist
     */
    bool Watchlist::removeSymbol(const std::string& symbol)
    {
        if (!_symbols.contains(symbol))
            return false;

        _symbols.remove(symbol);
        return true;
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
            [id](const Watchlist& watchlist) { return watchlist.getId() == id; }
        );
    }

}   // namespace finance
