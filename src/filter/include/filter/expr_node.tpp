#ifndef __FILTER__INCLUDE__FILTER__EXPR_NODE_TPP__
#define __FILTER__INCLUDE__FILTER__EXPR_NODE_TPP__

#include "expr_node.hpp"

namespace filter
{
    /**
     * @brief Create a conjunction (AND) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param left The left operand.
     * @param right The right operand.
     * @return A new conjunction (AND) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> makeAnd(Node<Leaf> left, Node<Leaf> right)
    {
        return AndNode<Leaf>{
            std::make_shared<Node<Leaf>>(std::move(left)),
            std::make_shared<Node<Leaf>>(std::move(right))
        };
    }

    /**
     * @brief Create a disjunction (OR) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param left The left operand.
     * @param right The right operand.
     * @return A new disjunction (OR) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> makeOr(Node<Leaf> left, Node<Leaf> right)
    {
        return OrNode<Leaf>{
            std::make_shared<Node<Leaf>>(std::move(left)),
            std::make_shared<Node<Leaf>>(std::move(right))
        };
    }

    /**
     * @brief Create a negation (NOT) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param operand The operand.
     * @return A new negation (NOT) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> makeNot(Node<Leaf> operand)
    {
        return NotNode<Leaf>{std::make_shared<Node<Leaf>>(std::move(operand))};
    }

    /**
     * @brief Create a conjunction (AND) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param left The left operand.
     * @param right The right operand.
     * @return A new conjunction (AND) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> operator&&(Node<Leaf> left, Node<Leaf> right)
    {
        return makeAnd(left, right);
    }

    /**
     * @brief Create a conjunction (AND) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param left The left operand.
     * @param right The right operand.
     * @return A new conjunction (AND) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> operator&=(Node<Leaf> left, Node<Leaf> right)
    {
        return makeAnd(left, right);
    }

    /**
     * @brief Create a disjunction (OR) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param left The left operand.
     * @param right The right operand.
     * @return A new disjunction (OR) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> operator||(Node<Leaf> left, Node<Leaf> right)
    {
        return makeOr(left, right);
    }

    /**
     * @brief Create a negation (NOT) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param operand The operand.
     * @return A new negation (NOT) filter expression.
     */
    template <typename Leaf>
    Node<Leaf> operator!(Node<Leaf> operand)
    {
        return makeNot(operand);
    }

    /**
     * @brief Check if a filter expression is empty.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param node The filter expression node.
     * @return True if the filter expression is empty, false otherwise.
     */
    template <typename Leaf>
    [[nodiscard]] bool isEmpty(const Node<Leaf>& node)
    {
        return std::holds_alternative<EmptyNode<Leaf>>(node);
    }

    /**
     * @brief Get the size of a filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param node The filter expression node.
     * @return The size of the filter expression.
     */
    template <typename Leaf>
    struct SizeVisitor
    {
        /**
         * @brief Get the size of an empty filter expression.
         *
         * @return The size of the empty filter expression.
         */
        size_t operator()(const EmptyNode<Leaf>& /*node*/) const { return 0; }

        /**
         * @brief Get the size of a leaf filter expression.
         *
         * @return The size of the leaf filter expression.
         */
        size_t operator()(const Leaf& /*node*/) const { return 1; }

        /**
         * @brief Get the size of a conjunction (AND) filter expression.
         *
         * @param node The conjunction (AND) filter expression node.
         * @return The size of the conjunction (AND) filter expression.
         */
        size_t operator()(const AndNode<Leaf>& node) const
        {
            return getSize(*node.left) + getSize(*node.right);
        }

        /**
         * @brief Get the size of a disjunction (OR) filter expression.
         *
         * @param node The disjunction (OR) filter expression node.
         * @return The size of the disjunction (OR) filter expression.
         */
        size_t operator()(const OrNode<Leaf>& node) const
        {
            return getSize(*node.left) + getSize(*node.right);
        }

        /**
         * @brief Get the size of a negation (NOT) filter expression.
         *
         * @param node The negation (NOT) filter expression node.
         * @return The size of the negation (NOT) filter expression.
         */
        size_t operator()(const NotNode<Leaf>& node) const
        {
            return getSize(*node.value);
        }
    };

    /**
     * @brief Get the size of a filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     * @param node The filter expression node.
     * @return The size of the filter expression.
     */
    template <typename Leaf>
    [[nodiscard]] size_t getSize(const Node<Leaf>& node)
    {
        return std::visit(SizeVisitor<Leaf>{}, node);
    }

}   // namespace filter

#endif   // __FILTER__INCLUDE__FILTER__EXPR_NODE_TPP__