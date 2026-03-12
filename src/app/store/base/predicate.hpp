#ifndef __APP__STORE__BASE__PREDICATE_HPP__
#define __APP__STORE__BASE__PREDICATE_HPP__

#include <functional>

#include "store_state.hpp"

namespace app
{

    /**
     * @brief Predicate for finding an entry by its ID
     *
     */
    template <typename Entry>
    struct Predicate
    {
        using EntryType = Entry;

        std::function<bool(const Entry&)> func;

        bool operator()(const Entry& entry) const { return func(entry); }

        Predicate operator&&(const Predicate& other) const
        {
            return Predicate{[first = *this, second = other](const Entry& entry)
                             { return first(entry) && second(entry); }};
        }

        Predicate operator||(const Predicate& other) const
        {
            return Predicate{[first = *this, second = other](const Entry& entry)
                             { return first(entry) || second(entry); }};
        }

        Predicate operator!() const
        {
            return Predicate{[pred = *this](const Entry& entry)
                             { return !pred(entry); }};
        }
    };

    template <typename Entry>
    Predicate<Entry> IsDeleted()
    {
        return Predicate<Entry>{[](const Entry& entry)
                                { return entry.state == StoreState::Deleted; }};
    }

}   // namespace app

#endif   // __APP__STORE__BASE__PREDICATE_HPP__