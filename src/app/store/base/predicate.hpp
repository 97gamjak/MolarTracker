#ifndef __APP__STORE__BASE__PREDICATE_HPP__
#define __APP__STORE__BASE__PREDICATE_HPP__

#include <functional>

namespace app
{

    /**
     * @brief Predicate for finding an entry by its ID
     *
     */
    template <typename Entry>
    struct Predicate
    {
        /// A function that takes an Entry and returns a boolean indicating
        /// whether the entry matches the predicate.
        std::function<bool(const Entry&)> func;

        bool operator()(const Entry& entry) const;

        Predicate operator&&(const Predicate& other) const;
        Predicate operator||(const Predicate& other) const;
        Predicate operator!() const;
    };

    template <typename Entry>
    Predicate<Entry> IsDeleted();

}   // namespace app

#ifndef __APP__STORE__BASE__PREDICATE_TPP__
#include "predicate.tpp"
#endif   // __APP__STORE__BASE__PREDICATE_TPP__

#endif   // __APP__STORE__BASE__PREDICATE_HPP__