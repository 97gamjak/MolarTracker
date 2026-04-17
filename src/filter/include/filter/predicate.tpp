#ifndef __FILTER__INCLUDE__FILTER__PREDICATE_TPP__
#define __FILTER__INCLUDE__FILTER__PREDICATE_TPP__

#include "expr_node.hpp"
#include "predicate.hpp"

namespace filter
{
    template <typename T>
    Predicate<T> makePredicate(PredicateFunc<T> func)
    {
        return Predicate<T>{std::move(func)};
    }

    template <typename T>
    struct PredicateEvaluator
    {
        const T& input;

        explicit PredicateEvaluator(const T& input) : input(input) {}

        ~PredicateEvaluator() = default;
        // delete constructor and assignment operator
        PredicateEvaluator(const PredicateEvaluator&)            = delete;
        PredicateEvaluator& operator=(const PredicateEvaluator&) = delete;
        PredicateEvaluator(PredicateEvaluator&&)                 = delete;
        PredicateEvaluator& operator=(PredicateEvaluator&&)      = delete;

        bool operator()(const PredicateFunc<T>& func) const
        {
            return func(input);
        }

        bool operator()(const AndNode<PredicateFunc<T>>& andNode) const
        {
            return evaluatePredicate(*andNode.left, input) &&
                   evaluatePredicate(*andNode.right, input);
        }

        bool operator()(const OrNode<PredicateFunc<T>>& orNode) const
        {
            return evaluatePredicate(*orNode.left, input) ||
                   evaluatePredicate(*orNode.right, input);
        }

        bool operator()(const NotNode<PredicateFunc<T>>& notNode) const
        {
            return !evaluatePredicate(*notNode.value, input);
        }

        bool operator()(const EmptyNode<PredicateFunc<T>>& /*emptyNode*/) const
        {
            return true;   // An empty predicate matches everything
        }
    };

    template <typename T>
    bool evaluatePredicate(const Predicate<T>& predicate, const T& input)
    {
        return std::visit(PredicateEvaluator<T>{input}, predicate);
    }

}   // namespace filter

#endif   // __FILTER__INCLUDE__FILTER__PREDICATE_TPP__