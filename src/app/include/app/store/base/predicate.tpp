#ifndef __APP__INCLUDE__APP__STORE__BASE__PREDICATE_TPP__
#define __APP__INCLUDE__APP__STORE__BASE__PREDICATE_TPP__

#include "predicate.hpp"
#include "store_state.hpp"

namespace app
{
    /**
     * @brief Evaluates the predicate function on the given entry, returning
     * true if the entry matches the predicate and false otherwise.
     *
     * @tparam Entry
     * @param entry
     * @return true
     * @return false
     */
    template <typename Entry>
    bool Predicate<Entry>::operator()(const Entry& entry) const
    {
        return func(entry);
    }

    /**
     * @brief Combines this predicate with another predicate using logical AND,
     * returning a new predicate that evaluates to true only if both predicates
     * are true for a given entry.
     *
     * @tparam Entry
     * @param other
     * @return Predicate<Entry>
     */
    template <typename Entry>
    Predicate<Entry> Predicate<Entry>::operator&&(const Predicate& other) const
    {
        return Predicate{[first = *this, second = other](const Entry& entry)
                         { return first(entry) && second(entry); }};
    }

    /**
     * @brief Combines this predicate with another predicate using logical OR,
     * returning a new predicate that evaluates to true if either predicate is
     * true for a given entry.
     *
     * @tparam Entry
     * @param other
     * @return Predicate<Entry>
     */
    template <typename Entry>
    Predicate<Entry> Predicate<Entry>::operator||(const Predicate& other) const
    {
        return Predicate{[first = *this, second = other](const Entry& entry)
                         { return first(entry) || second(entry); }};
    }

    /**
     * @brief Negates this predicate, returning a new predicate that evaluates
     * to true if this predicate is false for a given entry and false if this
     * predicate is true for a given entry.
     *
     * @tparam Entry
     * @return Predicate<Entry>
     */
    template <typename Entry>
    Predicate<Entry> Predicate<Entry>::operator!() const
    {
        return Predicate{[pred = *this](const Entry& entry)
                         { return !pred(entry); }};
    }

    /**
     * @brief Creates a predicate that checks if an entry is in the Deleted
     * state.
     *
     * @tparam Entry
     * @return Predicate<Entry>
     */
    template <typename Entry>
    Predicate<Entry> IsDeleted()
    {
        return Predicate<Entry>{[](const Entry& entry)
                                { return entry.state == StoreState::Deleted; }};
    }
}   // namespace app

#endif   // __APP__INCLUDE__APP__STORE__BASE__PREDICATE_TPP__