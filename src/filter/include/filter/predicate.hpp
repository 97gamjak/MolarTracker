#ifndef __FILTER__INCLUDE__FILTER__PREDICATE_HPP__
#define __FILTER__INCLUDE__FILTER__PREDICATE_HPP__

#include <functional>

#include "expr_node.hpp"

namespace filter
{
    template <typename T>
    using Predicate = Node<std::function<bool(const T&)>>;

    template <typename T>
    using PredicateFunc = std::function<bool(const T&)>;

    template <typename T>
    Predicate<T> makePredicate(PredicateFunc<T> func);

    template <typename T>
    bool evaluatePredicate(const Predicate<T>& predicate, const T& input);

}   // namespace filter

#ifndef __FILTER__INCLUDE__FILTER__PREDICATE_TPP__
#include "predicate.tpp"
#endif

#endif   // __FILTER__INCLUDE__FILTER__PREDICATE_HPP__