#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_HPP__
#define __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_HPP__

#include <vector>

#include "iterable.hpp"

/**
 * @brief A vector container that provides additional functionality on top of
 * std::vector.
 *
 * @tparam T The type of elements stored in the vector.
 */
template <typename T>
class Vector : public Iterable<std::vector<T>>
{
   private:
    using Iterable<std::vector<T>>::_items;

   public:
    using Iterable<std::vector<T>>::Iterable;
    Vector(std::initializer_list<T> items);

    template <std::ranges::input_range R>
    requires std::convertible_to<std::ranges::range_value_t<R>, T>
    explicit Vector(R&& range);

    virtual ~Vector() = default;

    const T& operator[](std::size_t index) const;

    auto front() const;

    virtual void add(const T& item);
    virtual void add(const std::vector<T>& items);
};

#ifndef __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_TPP__
#include "vector.tpp"
#endif

#endif   // __UTILS__INCLUDE__UTILS__CONTAINER__VECTOR_HPP__