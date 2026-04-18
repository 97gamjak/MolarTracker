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

    /**
     * @brief Visitor for evaluating a predicate expression against an input
     * value.
     *
     * @tparam T The type of the input value.
     */
    template <typename T>
    struct PredicateEvaluator
    {
        /// The input value to evaluate the predicate against.
        const T& input;

        /**
         * @brief Construct a new Predicate Evaluator object
         *
         * @param input
         */
        explicit PredicateEvaluator(const T& input) : input(input) {}

        ~PredicateEvaluator() = default;
        // delete constructor and assignment operator
        PredicateEvaluator(const PredicateEvaluator&)            = delete;
        PredicateEvaluator& operator=(const PredicateEvaluator&) = delete;
        PredicateEvaluator(PredicateEvaluator&&)                 = delete;
        PredicateEvaluator& operator=(PredicateEvaluator&&)      = delete;

        /**
         * @brief Evaluate a predicate function against the input value.
         *
         * @param func The predicate function to evaluate.
         * @return true if the predicate function returns true for the input
         * value, false otherwise.
         */
        bool operator()(const PredicateFunc<T>& func) const
        {
            return func(input);
        }

        /**
         * @brief Evaluate a conjunction (AND) of two predicate expressions
         * against the input value.
         *
         * @param andNode The conjunction (AND) node containing the two
         * predicate expressions to evaluate.
         * @return true if both predicate expressions return true for the input
         * value, false otherwise.
         */
        bool operator()(const AndNode<PredicateFunc<T>>& andNode) const
        {
            return evaluatePredicate(*andNode.left, input) &&
                   evaluatePredicate(*andNode.right, input);
        }

        /**
         * @brief Evaluate a disjunction (OR) of two predicate expressions
         * against the input value.
         *
         * @param orNode The disjunction (OR) node containing the two predicate
         * expressions to evaluate.
         * @return true if at least one of the predicate expressions returns
         * true for the input value, false otherwise.
         */
        bool operator()(const OrNode<PredicateFunc<T>>& orNode) const
        {
            return evaluatePredicate(*orNode.left, input) ||
                   evaluatePredicate(*orNode.right, input);
        }

        /**
         * @brief Evaluate a negation (NOT) of a predicate expression against
         * the input value.
         *
         * @param notNode The negation (NOT) node containing the predicate
         * expression to evaluate.
         * @return true if the predicate expression returns false for the input
         * value, false otherwise.
         */
        bool operator()(const NotNode<PredicateFunc<T>>& notNode) const
        {
            return !evaluatePredicate(*notNode.value, input);
        }

        /**
         * @brief Evaluate an empty predicate expression against the input
         * value.
         *
         * @return true, as an empty predicate matches all input values.
         */
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