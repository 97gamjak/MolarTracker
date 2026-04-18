#ifndef __FILTER__INCLUDE__FILTER__EXPR_NODE_HPP__
#define __FILTER__INCLUDE__FILTER__EXPR_NODE_HPP__

#include <memory>
#include <variant>

namespace filter
{
    template <typename Leaf>
    struct AndNode;

    template <typename Leaf>
    struct OrNode;

    template <typename Leaf>
    struct NotNode;

    /**
     * @brief A node representing an empty filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    struct EmptyNode
    {
    };

    /**
     * @brief A node representing a filter expression.
     *
     * @details ATTENTION: empty node needs to be the first alternative here!!!
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    using Node = std::variant<
        EmptyNode<Leaf>,
        Leaf,
        AndNode<Leaf>,
        OrNode<Leaf>,
        NotNode<Leaf>>;

    /**
     * @brief A pointer to a filter expression node.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    using NodePtr = std::shared_ptr<Node<Leaf>>;

    /**
     * @brief A node representing a binary filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    struct BiNode
    {
        /// The left child node.
        NodePtr<Leaf> left;
        /// The right child node.
        NodePtr<Leaf> right;
    };

    /**
     * @brief A node representing a single filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    struct SingleNode
    {
        /// The value of the node.
        NodePtr<Leaf> value;
    };

    /**
     * @brief A node representing a conjunction (AND) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    struct AndNode : BiNode<Leaf>
    {
    };

    /**
     * @brief A node representing a disjunction (OR) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    struct OrNode : BiNode<Leaf>
    {
    };

    /**
     * @brief A node representing a negation (NOT) filter expression.
     *
     * @tparam Leaf The type of the leaf nodes.
     */
    template <typename Leaf>
    struct NotNode : SingleNode<Leaf>
    {
    };

    template <typename Leaf>
    Node<Leaf> makeAnd(Node<Leaf> left, Node<Leaf> right);

    template <typename Leaf>
    Node<Leaf> makeOr(Node<Leaf> left, Node<Leaf> right);

    template <typename Leaf>
    Node<Leaf> makeNot(Node<Leaf> operand);

    template <typename Leaf>
    Node<Leaf> operator&&(Node<Leaf> left, Node<Leaf> right);

    template <typename Leaf>
    Node<Leaf>& operator&=(Node<Leaf>& left, const Node<Leaf>& right);

    template <typename Leaf>
    Node<Leaf> operator||(Node<Leaf> left, Node<Leaf> right);

    template <typename Leaf>
    Node<Leaf> operator!(Node<Leaf> operand);

    template <typename Leaf>
    [[nodiscard]] bool isEmpty(const Node<Leaf>& node);

    template <typename Leaf>
    [[nodiscard]] size_t getSize(const Node<Leaf>& node);

}   // namespace filter

#ifndef __FILTER__INCLUDE__FILTER__EXPR_NODE_TPP__
#include "expr_node.tpp"
#endif

#endif   // __FILTER__INCLUDE__FILTER__EXPR_NODE_HPP__